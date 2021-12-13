#include "ResourceTrackerManager.hpp"
#include "object.hpp"



namespace asapi
{
	ResourceTrackerManager::~ResourceTrackerManager()
	{
		CleanResourceTrackersContainer();
	}

	void ResourceTrackerManager::Init(const char* projectPath, IterateOverDirtyResourceTrackersCallbackType callback)
	{
		m_callback = callback;

		SetProjectPath(projectPath);

		srand( time( NULL ) );
	}

	void ResourceTrackerManager::SetProjectPath(const char* projectPath)
	{
		CleanResourceTrackersContainer();

		s_assetsDirectoryPath = projectPath;

		if( s_assetsDirectoryPath.back() != '/' )
			s_assetsDirectoryPath += "/";

		s_assetsDirectoryPath += "assets/";

		s_projectDirectoryPath = projectPath;

		ResourceTracker::SetProjectPath(projectPath);


		DeserializeResourceTrackerContainerFromDisk();
	}

	void ResourceTrackerManager::DeserializeResourceTrackerContainerFromDisk()
	{

		std::string 						path = s_projectDirectoryPath + RESOURCE_TRACKERS_DIR;
		std::vector< std::string > 			paths;
		char pathBuff[MAX_PATH_SIZE];


		ListFiles(paths, {".res.json"}, ListingStrategy::whitelist, path.c_str());


		v_ResourceTrackers.resize( paths.size() );


		for(int i=0; i<paths.size(); ++i)
		{
			FILE::MMAP input;
			
			std::string resourceTrackerPath = s_projectDirectoryPath + RESOURCE_TRACKERS_DIR;
			resourceTrackerPath += "/";
			resourceTrackerPath += paths[i];

			input.InitForRead( resourceTrackerPath.c_str() );

			//log::debug << "resource path: " << resourceTrackerPath << std::endl;

			p_JSONSerializer.assignData( (char*)input.Data(), input.Size() );

			p_JSONSerializer.Deserialize( &v_ResourceTrackers[i] );

			input.Close();
		}
	}

	ResourceTracker* FindResource(std::vector<ResourceTracker>& where, const ResourceTracker& what)
	{
		ResourceTracker* ret = 0;

		for(int i=0; i<where.size(); ++i)
		{
			if( where[i].CmpPath(what) )
			{
				ret = &where[i];
				break;
			}
		}

		if( ret==0 )
		{
			for(int i=0; i<where.size(); ++i)
			{
				if( where[i].CmpContent(what) )
				{
					ret = &where[i];
					break;
				}
			}
		}

		return ret;
	}

	void ResourceTrackerManager::RefreshResources()
	{
		std::vector< std::string > 			paths;
		std::vector< ResourceTracker > 		upToDateResources;
		char pathBuff[MAX_PATH_SIZE];

		ListFiles(paths, {".res.json", ".bin"}, ListingStrategy::blacklist, s_assetsDirectoryPath.c_str());

		upToDateResources.resize( paths.size() );


		for(int i=0; i<paths.size(); ++i)
		{
			upToDateResources[i].Init( paths[i].c_str() );
		}

		//transfer uniqueID to new resource set, so the links are preserved
		for(int i=0; i<v_ResourceTrackers.size(); ++i)
		{
			ResourceTracker* res = FindResource( upToDateResources, v_ResourceTrackers[i] );

			if( res != nullptr )
			{
				res->ObtainResourceOwnership(v_ResourceTrackers[i]);

				if( !res->CmpContent( v_ResourceTrackers[i] ) )
				{
					res->SetContentDirty( true );
				}
				else
				{
					res->SetContentDirty( false );
				}
			}
		}

		v_ResourceTrackers = std::move( upToDateResources );

		IterateOverDirtyResourceTrackers();


		for(int i=0; i<v_ResourceTrackers.size(); ++i)
		{
			FILE::STREAM out;
			
			std::string resourceTrackerPath = v_ResourceTrackers[i].GetResourceTrackerPath();

			//log::debug << "resource path: " << resourceTrackerPath << std::endl;

			p_JSONSerializer.clear();

			p_JSONSerializer.Serialize( &v_ResourceTrackers[i] );

			out.InitForWrite( resourceTrackerPath.c_str() );
			out.Write( p_JSONSerializer.c_str(), p_JSONSerializer.size() );
		}
	}

	ResourceTracker* ResourceTrackerManager::FindResourceByContentHash(const std::string& content_hash)
	{
		for(int i=0; i<v_ResourceTrackers.size(); ++i)
		{
			if( strcmp( v_ResourceTrackers[i].m_content_hash.c_str(), content_hash.c_str() ) == 0 )
			{
				return &v_ResourceTrackers[i];
			}
		}

		return nullptr;
	}
	ResourceTracker* ResourceTrackerManager::FindResourceByFilename(const std::string& filename)
	{
		for(int i=0; i<v_ResourceTrackers.size(); ++i)
		{
			if( strcmp( v_ResourceTrackers[i].m_filename.c_str(), filename.c_str() ) == 0 )
			{
				return &v_ResourceTrackers[i];
			}
		}

		return nullptr;
	}
	ResourceTracker* ResourceTrackerManager::FindResourceByResourceID(const uint64_t& resourceID)
	{
		for(int i=0; i<v_ResourceTrackers.size(); ++i)
		{
			if( v_ResourceTrackers[i].m_resourceID.ID() == resourceID )
			{
				return &v_ResourceTrackers[i];
			}
		}

		return nullptr;
	}
	ResourceTracker* ResourceTrackerManager::FindResourceByResourceID(const UniqueID& resourceID)
	{
		for(int i=0; i<v_ResourceTrackers.size(); ++i)
		{
			if( v_ResourceTrackers[i].m_resourceID.ID() == resourceID.ID() )
			{
				return &v_ResourceTrackers[i];
			}
		}

		return nullptr;
	}

	void ResourceTrackerManager::IterateOverDirtyResourceTrackers()
	{

		for(int i=0; i<v_ResourceTrackers.size(); i++)
		{
			if( v_ResourceTrackers[i].IsContentDirty() )
			{
				std::vector<SubResourceData> tmpVec;
				const bool succesfulyProcessed = m_callback( &(v_ResourceTrackers[i]), s_assetsDirectoryPath.c_str(), tmpVec);

				v_ResourceTrackers[i].v_subresources.clear();
				for(int j=0; j<tmpVec.size(); ++j)
				{
					log::debug << "DUPAASsafasfdgasdf " << i << " " << (v_ResourceTrackers[i]).m_filename.c_str() << std::endl;
					
					//v_ResourceTrackers[i].v_subresources.push_back( new SerializableSubResourceData( tmpVec[j] ) );
					v_ResourceTrackers[i].v_subresources.push_back( SerializableSubResourceData::AllocateAndInit( bfu::StdAllocatorMemBlock::GetMemBlock() ) );
					*(SerializableSubResourceData*)v_ResourceTrackers[i].v_subresources.back() = std::move( tmpVec[j] );
				}

				v_ResourceTrackers[i].SetContentDirty( !succesfulyProcessed );  //if processet succesfully then is NOT dirty
			}
		}
	}

	void ResourceTrackerManager::CleanResourceTrackersContainer()
	{
		for(int i=0; i<v_ResourceTrackers.size(); ++i)
		{
			v_ResourceTrackers[i].DetachFromResourceOnApplicationCLose();
		}
		v_ResourceTrackers.clear();
	}


	bfu::stream& operator<<(bfu::stream& os, const ResourceTrackerManager& res)
	{
		for(int i=0; i<res.v_ResourceTrackers.size(); ++i)
		{
			os << res.v_ResourceTrackers[i];
		}
		return os;
	}
}