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
				(strstr(entry->d_name, ".asset.json") == 0) &&
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


	void AssetSystem::Init(char** resourcesPath)
	{
		ps_resourcesDirectoryPath = resourcesPath;

		srand( time( NULL ) );
	}

	ResourceTracker* FindResource(std::vector<ResourceTracker>& where, const ResourceTracker& what)
	{
		ResourceTracker* ret = 0;

		for(int i=0; i<where.size(); ++i)
		{
			if( where[i]==what )
			{
				ret = &where[i];
				break;
			}
		}
		return ret;
	}

	void AssetSystem::RefreshResources()
	{
		std::vector< std::string > 			paths;
		std::vector< ResourceTracker > 		upToDateResources;

		ListFiles(paths, *ps_resourcesDirectoryPath);

		upToDateResources.resize( paths.size() );


		for(int i=0; i<paths.size(); ++i)
		{
			upToDateResources[i].Init( paths[i].c_str() );
		}

		for(int i=0; i<v_ResourceTrackers.size(); ++i)
		{
			ResourceTracker* res = FindResource( upToDateResources, v_ResourceTrackers[i] );

			if( res != nullptr )
			{
				*res = std::move( v_ResourceTrackers[i] );
			}
		}

		v_ResourceTrackers = std::move( upToDateResources );
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