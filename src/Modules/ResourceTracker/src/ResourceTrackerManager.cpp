#include "ResourceTrackerManager.hpp"
#include "object.hpp"



namespace asapi
{
	ResourceTrackerManagerBase::~ResourceTrackerManagerBase()
	{
		CleanResourceTrackersContainer();
	}

	void ResourceTrackerManagerBase::Init(const char* projectPath)
	{
		SetProjectPath(projectPath);

		srand( time( NULL ) );
	}

	void ResourceTrackerManagerBase::SetProjectPath(const char* projectPath)
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

	void ResourceTrackerManagerBase::DeserializeResourceTrackerContainerFromDisk()
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

	void ResourceTrackerManagerBase::RefreshResources()
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

	ResourceTracker* ResourceTrackerManagerBase::FindResourceByContentHash(const std::string& content_hash)
	{
		for(int i=0; i<v_ResourceTrackers.size(); ++i)
		{
			if( strcmp( v_ResourceTrackers[i].GetContentHash().c_str(), content_hash.c_str() ) == 0 )
			{
				return &v_ResourceTrackers[i];
			}
		}

		return nullptr;
	}
	ResourceTracker* ResourceTrackerManagerBase::FindResourceByFilename(const std::string& filename)
	{
		for(int i=0; i<v_ResourceTrackers.size(); ++i)
		{
			if( strcmp( v_ResourceTrackers[i].GetFilename().c_str(), filename.c_str() ) == 0 )
			{
				return &v_ResourceTrackers[i];
			}
		}

		return nullptr;
	}
	ResourceTracker* ResourceTrackerManagerBase::FindResourceByResourceID(const uint64_t& resourceID)
	{
		for(int i=0; i<v_ResourceTrackers.size(); ++i)
		{
			if( v_ResourceTrackers[i].GetResourceID() == resourceID )
			{
				return &v_ResourceTrackers[i];
			}
		}

		return nullptr;
	}
	ResourceTracker* ResourceTrackerManagerBase::FindResourceByResourceID(const UniqueID& resourceID)
	{
		for(int i=0; i<v_ResourceTrackers.size(); ++i)
		{
			if( v_ResourceTrackers[i].GetResourceID() == resourceID.ID() )
			{
				return &v_ResourceTrackers[i];
			}
		}

		return nullptr;
	}

	void ResourceTrackerManagerBase::IterateOverDirtyResourceTrackers()
	{
		for(int i=0; i<v_ResourceTrackers.size(); i++)
		{
			if( v_ResourceTrackers[i].IsContentDirty() )
			{
				std::vector<SubResourceData> tmpVec;
				asapi::FILE::MMAP _in;
				_in.InitForRead( v_ResourceTrackers[i].GetFullPath().c_str() );
				
				const bool resourceBinaryChanged = ProcessResource( v_ResourceTrackers[i]
																, &_in
																, s_projectDirectoryPath.c_str()
																, &tmpVec );

				v_ResourceTrackers[i].MoveSubresources( &tmpVec );

				v_ResourceTrackers[i].SetContentDirty( !resourceBinaryChanged );  //if processet succesfully then is NOT dirty
			}
		}
	}

	void ResourceTrackerManagerBase::CleanResourceTrackersContainer()
	{
		for(int i=0; i<v_ResourceTrackers.size(); ++i)
		{
			v_ResourceTrackers[i].DetachFromResourceOnApplicationCLose();
		}
		v_ResourceTrackers.clear();
	}


	bfu::stream& operator<<(bfu::stream& os, const ResourceTrackerManagerBase& res)
	{
		for(int i=0; i<res.v_ResourceTrackers.size(); ++i)
		{
			os << res.v_ResourceTrackers[i];
		}
		return os;
	}

	int ResourceTrackerManagerBase::CountSubresources() 
	{ 
		int count = 0;

		for(int i=0; i<v_ResourceTrackers.size(); ++i)
		{
			count += v_ResourceTrackers[i].CountSubresources();
		}

		return count; 
	}
}