#include "ResourceSystem.hpp"
#include <dirent.h> 
#include <sys/stat.h>


namespace asapi{

	void ResourceSystem::Init(const int argc, const char** argv)
	{
		const char* path = "--wrong path--";

		bool startWithJSON = false;
		for(int i=1; i<argc; ++i)
		{
			#ifdef IS_PLAYER
			if( strcmp(argv[i], "-json") == 0 )
			{
				startWithJSON = true;
			}
			else
			#endif
			if( strcmp(argv[i], "-path") == 0 )
			{
				if(argc > i) // if there is next argument
				{
					SetProjectPath(argv[i+1]);
					path = argv[i+1];
				}
				else
				{
					log::error << "You did not suplied path for project. Run '?' fur help." << std::endl;
				}
			}
		}
		#ifdef IS_PLAYER
		if( !(startWithJSON ? OpenProject() : LoadRootMMP() ) )
		{
			log::error << "Could not open project at path " << path << std::endl;
		}
		#endif
	}
	void ResourceSystem::SetProjectPath(const char* path)
	{
		strncpy(m_ProjectPath, path, MAX_PATH );
		m_ProjectPath[MAX_PATH-1] = '\0';


		#ifdef IS_EDITOR
		RefreshResources();
		#endif
	}

	#ifdef IS_EDITOR

	void ScanDirForPaths(std::vector<std::string>& out, const char* dirname)
	{
		int i;
		DIR* d_fh;
		struct dirent* entry;
		char longest_name[MAX_PATH];

		while( (d_fh = opendir(dirname)) == NULL) {
			log::error << "Couldn't open directory: " << dirname << std::endl;
			return;
		}

		while((entry=readdir(d_fh)) != NULL) 
		{
			/* Don't descend up the tree or include the current directory */
			if(strncmp(entry->d_name, "..", 2) != 0 &&
			strncmp(entry->d_name, ".", 1) != 0)
			{
				/* Prepend the current directory and recurse */
				strncpy(longest_name, dirname, MAX_PATH-1);
				strncat(longest_name, "/", MAX_PATH-1);
				strncat(longest_name, entry->d_name, MAX_PATH-1);

				/* If it's a directory print it's name and recurse into it */
				if (entry->d_type == DT_DIR) 
				{
					ScanDirForPaths(out, longest_name);
				}
				else //if not dir
				{
					out.emplace_back(longest_name);
				}
			}
		}

		closedir(d_fh);
	}

	bool IsInternalAssetUpToDate(const char* path, char* outPath)
	{
		struct stat attribInt;
		struct stat attribExt;

		strncpy(outPath, path, MAX_PATH-1);
		outPath[MAX_PATH-1] = '\0';

		char* substr = strstr(outPath, "/assets_ext/") + strlen("/assets_");

		memcpy(substr, "int", 3);


    	stat(path, &attribExt);
    	stat(outPath, &attribInt);

    	return attribExt.st_mtime < attribInt.st_mtime;
	}

	void ResourceSystem::RefreshResources()
	{
		char buff[MAX_PATH];

		if(strcmp(m_ProjectPath, ".")==0)
			return;

		v_TexturesPaths.clear();
		v_ShadersPaths.clear();
		v_MeshesPaths.clear();

		char dir_path[MAX_PATH];

		strncpy(dir_path, m_ProjectPath, MAX_PATH-1);
		strncat(dir_path, "/assets_ext/textures", MAX_PATH-1);
		ScanDirForPaths(v_TexturesPaths, dir_path);

		strncpy(dir_path, m_ProjectPath, MAX_PATH-1);
		strncat(dir_path, "/assets_ext/shaders", MAX_PATH-1);
		ScanDirForPaths(v_ShadersPaths, dir_path);

		strncpy(dir_path, m_ProjectPath, MAX_PATH-1);
		strncat(dir_path, "/assets_ext/meshes", MAX_PATH-1);
		ScanDirForPaths(v_MeshesPaths, dir_path);

		log::debug << "Scanning for files showed follow files in project folder:" << std::endl;
		for(int i=0; i<v_TexturesPaths.size(); ++i)
		{
			std::cout << v_TexturesPaths[i] 
						<< (IsInternalAssetUpToDate(v_TexturesPaths[i].c_str(), buff) ? " up to date" : " need to update" )
						<<  std::endl;
			Texture::Compile(buff, v_TexturesPaths[i].c_str());
		}
		for(int i=0; i<v_ShadersPaths.size(); ++i)
		{
			std::cout << v_ShadersPaths[i] 
						<< (IsInternalAssetUpToDate(v_ShadersPaths[i].c_str(), buff) ? " up to date" : " need to update" )
						<<  std::endl;
		}
		for(int i=0; i<v_MeshesPaths.size(); ++i)
		{
			std::cout << v_MeshesPaths[i] 
						<< (IsInternalAssetUpToDate(v_MeshesPaths[i].c_str(), buff) ? " up to date" : " need to update" )
						<<  std::endl;
		}
	}
	#endif

}