#include "ResourceSystem.hpp"
#include <dirent.h> 
#include <sys/stat.h>
#include "Systems.hpp"

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
		SceneSystem* sceneSys = &SYSTEMS::GetObject().SCENE;
		if( !(startWithJSON ? sceneSys->OpenProject() : sceneSys->LoadRootMMP() ) )
		{
			log::error << "Could not open project at path " << path << std::endl;
		}
		#endif
	}
	void ResourceSystem::SetProjectPath(const char* path)
	{
		strncpy(m_ProjectPath, path, MAX_PATH_SIZE );
		m_ProjectPath[MAX_PATH_SIZE-1] = '\0';


		#ifdef IS_EDITOR
		RefreshResources();
		#endif
	}

	#ifdef IS_EDITOR

	void ScanDirForPaths(std::vector<std::string>& out, const char* dirname, const char* fileExtensionFilter = 0)
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
				((fileExtensionFilter != 0) ? (strstr(entry->d_name, fileExtensionFilter) != 0) : true) ) 
			{
				/* Prepend the current directory and recurse */
				strncpy(longest_name, dirname, MAX_PATH_SIZE-1);
				strncat(longest_name, "/", MAX_PATH_SIZE-1);
				strncat(longest_name, entry->d_name, MAX_PATH_SIZE-1);

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


		strncpy(outPath, path, MAX_PATH_SIZE-1);
		outPath[MAX_PATH_SIZE-1] = '\0';

		char* substr = strstr(outPath, "/assets_ext/") + strlen("/assets_");

		memcpy(substr, "int", 3);


		bool ret = false;
    	if( stat(path, &attribExt)!=0 )
    	{
    		log::error << "Could not find file " << path << std::endl;
    	}
    	else if( stat(outPath, &attribInt)!=0 )
    	{
    		log::info << "Could not find file " << outPath << " it will be recompile from assets_ext" << std::endl;
    	}
    	else
    	{
    		ret = attribExt.st_mtime < attribInt.st_mtime;
    	}
    	return ret;
	}

	void ResourceSystem::RefreshResources()
	{
		char buff[MAX_PATH_SIZE];

		if(strcmp(m_ProjectPath, ".")==0)
			return;

		std::vector<std::string> TexturesPaths;
		std::vector<std::string> ShadersPaths;
		std::vector<std::string> MeshesPaths;

		char dir_path[MAX_PATH_SIZE];

		strncpy(dir_path, m_ProjectPath, MAX_PATH_SIZE-1);
		strncat(dir_path, "/assets_ext/textures", MAX_PATH_SIZE-1);
		ScanDirForPaths(TexturesPaths, dir_path);

		strncpy(dir_path, m_ProjectPath, MAX_PATH_SIZE-1);
		strncat(dir_path, "/assets_ext/shaders", MAX_PATH_SIZE-1);
		ScanDirForPaths(ShadersPaths, dir_path);

		strncpy(dir_path, m_ProjectPath, MAX_PATH_SIZE-1);
		strncat(dir_path, "/assets_ext/meshes", MAX_PATH_SIZE-1);
		ScanDirForPaths(MeshesPaths, dir_path);

		//log::debug << "Scanning for files showed follow files in project folder:" << std::endl;
		for(int i=0; i<TexturesPaths.size(); ++i)
		{
			if( ! IsInternalAssetUpToDate(TexturesPaths[i].c_str(), buff) )
			{
				Texture::Compile(buff, TexturesPaths[i].c_str());
			}
		}
		for(int i=0; i<ShadersPaths.size(); ++i)
		{
			if( ! IsInternalAssetUpToDate(ShadersPaths[i].c_str(), buff) )
			{
				Shader::Compile(buff, ShadersPaths[i].c_str());
			}
		}
		for(int i=0; i<MeshesPaths.size(); ++i)
		{
			if( ! IsInternalAssetUpToDate(MeshesPaths[i].c_str(), buff) )
			{
				Mesh::Compile(buff, MeshesPaths[i].c_str());
			}
		}

		v_TexturesPaths.clear();
		strncpy(dir_path, m_ProjectPath, MAX_PATH_SIZE-1);
		strncat(dir_path, "/assets_int/textures", MAX_PATH_SIZE-1);
		ScanDirForPaths(v_TexturesPaths, dir_path);

		v_ShadersPaths.clear();
		strncpy(dir_path, m_ProjectPath, MAX_PATH_SIZE-1);
		strncat(dir_path, "/assets_int/shaders", MAX_PATH_SIZE-1);
		ScanDirForPaths(v_ShadersPaths, dir_path, "vert.glsl");

		v_MeshesPaths.clear();
		strncpy(dir_path, m_ProjectPath, MAX_PATH_SIZE-1);
		strncat(dir_path, "/assets_int/meshes", MAX_PATH_SIZE-1);
		ScanDirForPaths(v_MeshesPaths, dir_path, ".mmp");
	}
	#endif

}