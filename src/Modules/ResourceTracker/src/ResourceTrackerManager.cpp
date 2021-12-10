#include "ResourceTrackerManager.hpp"
#include "object.hpp"



namespace asapi
{
	void ResourceTrackerManager::Init(const char* projectPath, IterateOverDirtyResourceTrackersCallbackType callback)
	{
		SetProjectPath(projectPath);

		m_callback = callback;

		srand( time( NULL ) );
	}

	void ResourceTrackerManager::SetProjectPath(const char* projectPath)
	{
		s_assetsDirectoryPath = projectPath;

		if( s_assetsDirectoryPath.back() != '/' )
			s_assetsDirectoryPath += "/";

		s_assetsDirectoryPath += "assets/";

		ResourceTracker::SetProjectPath(projectPath);
		RefreshResources();
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

		ListFiles(paths, s_assetsDirectoryPath.c_str(), {".res.json", ".bin"}, ListingStrategy::blacklist);

		upToDateResources.resize( paths.size() );


		for(int i=0; i<paths.size(); ++i)
		{
			upToDateResources[i].Init( paths[i].c_str() );
		}

		//transfer uniqueID to new resource set, so the links are preserved
		for(int i=0; i<v_ResourceTrackers.size(); ++i)
		{
			v_ResourceTrackers[i].MarkOutdated();

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
				std::vector<std::string> tmpVec;
				const bool succesfulyProcessed = m_callback( &(v_ResourceTrackers[i]), s_assetsDirectoryPath.c_str(), tmpVec);

				v_ResourceTrackers[i].v_resourceIDs.clear();
				for(int j=0; j<tmpVec.size(); ++j)
				{
					log::debug << "DUPAASsafasfdgasdf " << i << " " << (v_ResourceTrackers[i]).m_filename.c_str() << std::endl;
					v_ResourceTrackers[i].v_resourceIDs.push_back( tmpVec[j] );
				}

				v_ResourceTrackers[i].SetContentDirty( !succesfulyProcessed );  //if processet succesfully then is NOT dirty
			}
		}
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