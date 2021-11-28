#include "ResourceSystem2.hpp"
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


	void ResourceSystem2::Init(char** resourcesPath)
	{
		ps_resourcesDirectoryPath = resourcesPath;
	}

	void ResourceSystem2::RefreshResources()
	{
		std::vector< std::string > paths;

		ListFiles(paths, *ps_resourcesDirectoryPath);

		for(int i=0; i<paths.size(); ++i)
		{
			ResourceTracker newres;
			newres.Init( paths[i].c_str() );

			log::info << newres << std::endl;
		}
	}
}