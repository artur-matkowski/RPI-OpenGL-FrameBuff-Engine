#include "AssetSystem.hpp"
#include "object.hpp"

#include <dirent.h> 
#include <sys/stat.h>


namespace asapi
{
	void ListFiles(std::vector< std::string >& out, const char* dirname)
	{
		int i;
		DIR* d_fh;
		struct dirent* entry;
		char longest_name[MAX_PATH_SIZE];

		mkdir(dirname, 755);

		if( (d_fh = opendir(dirname)) == NULL) 
		{
			log::error << "Couldn't open directory, errno: " << errno << "\n\tDirname: " << dirname << " " << std::endl;
			return;
		}

		// if( (entry=readdir(d_fh)) == NULL )
		// {
		// 	log::error << "readdir error: " << errno << std::endl;
		// 	return;
		// }
		
		
		while( (entry=readdir(d_fh)) != NULL )
		{
			/* Don't descend up the tree or include the current directory */
			if( (strncmp(entry->d_name, "..", 2) != 0) &&
				(strncmp(entry->d_name, ".", 1) != 0) &&
				(strstr(entry->d_name, ".res.json") == 0) &&
				(strstr(entry->d_name, ".asset.bin") == 0) )
			{
				/* Prepend the current directory and recurse */
				strncpy(longest_name, dirname, MAX_PATH_SIZE-1);
				strncat(longest_name, "/", MAX_PATH_SIZE-1);
				strncat(longest_name, entry->d_name, MAX_PATH_SIZE-1);

				/* If it's a directory print it's name and recurse into it */
				if (entry->d_type == DT_DIR) 
				{
					ListFiles(out, longest_name);
				}
				else //if not dir
				{
					out.emplace_back(longest_name);
				}
			}
		}
		
		
		

		closedir(d_fh);
	}

	void AssetSystem::Init(const char* projectPath)
	{
		SetProjectPath(projectPath);

		srand( time( NULL ) );
	}

	void AssetSystem::SetProjectPath(const char* projectPath)
	{
		s_assetsDirectoryPath = projectPath;

		if( s_assetsDirectoryPath.back() != '/' )
			s_assetsDirectoryPath += "/";

		s_assetsDirectoryPath += "assets/";

		ResourceTracker::SetProjectPath(projectPath);
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
/*
	void GetDirectory(char* buff, const char* fullpath, uint32_t fullPathSize)
	{
		strncpy(buff, fullpath, std::min(fullPathSize, (uint32_t)MAX_PATH_SIZE));

		int index = (int)std::min(fullPathSize, (uint32_t)MAX_PATH_SIZE)-1;

		while( buff[index]!='/' && index>=0 )
		{
			buff[index]='\0';
			--index;
		}
	}
*/
	void AssetSystem::RefreshResources()
	{
		std::vector< std::string > 			paths;
		std::vector< ResourceTracker > 		upToDateResources;
		char pathBuff[MAX_PATH_SIZE];

		ListFiles(paths, s_assetsDirectoryPath.c_str());

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
					res->SetDirty( true );
				}
			}
		}

		v_ResourceTrackers = std::move( upToDateResources );


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


	bfu::stream& operator<<(bfu::stream& os, const AssetSystem& res)
	{
		for(int i=0; i<res.v_ResourceTrackers.size(); ++i)
		{
			os << res.v_ResourceTrackers[i];
		}
		return os;
	}
}