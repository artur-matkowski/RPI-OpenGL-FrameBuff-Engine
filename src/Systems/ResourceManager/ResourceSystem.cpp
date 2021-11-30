#include "ResourceSystem.hpp"
#include <dirent.h> 
#include <sys/stat.h>
#include "Systems.hpp"

#ifdef IS_EDITOR
#include "imgui.h"
#endif

namespace asapi{

	void ResourceSystem::Init(const int argc, const char** argv)
	{
		p_materialsMemBlock = SYSTEMS::GetObject().RENDERER.GetMaterialsMemBlock();


		const char* path = "--wrong path--";

		for(int i=1; i<argc; ++i)
		{
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
		if( !sceneSys->OpenProject() )
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

	void ListFiles(std::vector<std::string>& out, const char* dirname, const char* fileExtensionFilter = 0)
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

	void RemoveExtensions(std::vector<std::string>& in)
	{
		for(int i=0; i<in.size(); ++i)
		{
			for(int j=in[i].size()-1; j>0; --j)
			{
				if(in[i][j]=='.')
					in[i][j] = '\0';
				else if(in[i][j]=='/')
					break;
			}
		}
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

	void ResourceSystem::BuildDirectoryStructure()
	{
		char dir_path[MAX_PATH_SIZE];
		
		strncpy(dir_path, m_ProjectPath, MAX_PATH_SIZE-1);
		strncat(dir_path, "/assets_int", MAX_PATH_SIZE-1);
		mkdir(dir_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

		strncpy(dir_path, m_ProjectPath, MAX_PATH_SIZE-1);
		strncat(dir_path, "/assets_int/textures", MAX_PATH_SIZE-1);
		mkdir(dir_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

		strncpy(dir_path, m_ProjectPath, MAX_PATH_SIZE-1);
		strncat(dir_path, "/assets_int/materials", MAX_PATH_SIZE-1);
		mkdir(dir_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

		strncpy(dir_path, m_ProjectPath, MAX_PATH_SIZE-1);
		strncat(dir_path, "/assets_int/shaders", MAX_PATH_SIZE-1);
		mkdir(dir_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

		strncpy(dir_path, m_ProjectPath, MAX_PATH_SIZE-1);
		strncat(dir_path, "/assets_int/meshes", MAX_PATH_SIZE-1);
		mkdir(dir_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}





	void ResourceSystem::RefreshResources()
	{
		char buff[MAX_PATH_SIZE];

		if(strcmp(m_ProjectPath, ".")==0)
			return;


		std::vector<std::string> TexturesPaths;
		std::vector<std::string> MaterialsPaths;
		std::vector<std::string> ShadersPaths;
		std::vector<std::string> MeshesPaths;

		char dir_path[MAX_PATH_SIZE];


		strncpy(dir_path, m_ProjectPath, MAX_PATH_SIZE-1);
		strncat(dir_path, "/assets_ext/textures", MAX_PATH_SIZE-1);
		ScanDirForPaths(TexturesPaths, dir_path);

		strncpy(dir_path, m_ProjectPath, MAX_PATH_SIZE-1);
		strncat(dir_path, "/assets_ext/materials", MAX_PATH_SIZE-1);
		ScanDirForPaths(MaterialsPaths, dir_path);

		strncpy(dir_path, m_ProjectPath, MAX_PATH_SIZE-1);
		strncat(dir_path, "/assets_ext/shaders", MAX_PATH_SIZE-1);
		ScanDirForPaths(ShadersPaths, dir_path);

		strncpy(dir_path, m_ProjectPath, MAX_PATH_SIZE-1);
		strncat(dir_path, "/assets_ext/meshes", MAX_PATH_SIZE-1);
		ScanDirForPaths(MeshesPaths, dir_path);

		BuildDirectoryStructure();

		//log::debug << "Scanning for files showed follow files in project folder:" << std::endl;
		for(int i=0; i<TexturesPaths.size(); ++i)
		{
			if( ! IsInternalAssetUpToDate(TexturesPaths[i].c_str(), buff) )
			{
				Texture::Compile(buff, TexturesPaths[i].c_str());
			}
		}
		for(int i=0; i<MaterialsPaths.size(); ++i)
		{
			if( ! IsInternalAssetUpToDate(MaterialsPaths[i].c_str(), buff) )
			{
				MaterialType::Compile(buff, MaterialsPaths[i].c_str());
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

		v_MaterialsPaths.clear();
		strncpy(dir_path, m_ProjectPath, MAX_PATH_SIZE-1);
		strncat(dir_path, "/assets_int/materials", MAX_PATH_SIZE-1);
		ScanDirForPaths(v_MaterialsPaths, dir_path);
		RemoveExtensions(v_MaterialsPaths);

		v_ShadersPaths.clear();
		strncpy(dir_path, m_ProjectPath, MAX_PATH_SIZE-1);
		strncat(dir_path, "/assets_int/shaders", MAX_PATH_SIZE-1);
		ScanDirForPaths(v_ShadersPaths, dir_path, "vert.glsl");
		RemoveExtensions(v_ShadersPaths);

		v_MeshesPaths.clear();
		strncpy(dir_path, m_ProjectPath, MAX_PATH_SIZE-1);
		strncat(dir_path, "/assets_int/meshes", MAX_PATH_SIZE-1);
		ScanDirForPaths(v_MeshesPaths, dir_path, ".mmp");
	}

	void ResourceSystem::OnGUI()
	{
		static char  namebuff[MATERIAL_MAX_NAME_LENGTH] = {'\0'};
		bool doRefreshResources = false;

		if (!ImGui::CollapsingHeader("Materials"))
        	return;

		for(auto it=m_materials.begin(); it!=m_materials.end(); ++it)
		{
			if (ImGui::TreeNode( it->second->GetMaterialName() ))
		    {
		    	if (ImGui::TreeNode( "Rename Material" ))
		    	{
		    		if(namebuff[0]=='\0')
		    			strncpy(namebuff, it->second->GetMaterialName(), MATERIAL_MAX_NAME_LENGTH-1);

	    			if( ImGui::InputText( "Material name", namebuff, MATERIAL_MAX_NAME_LENGTH-1, ImGuiInputTextFlags_EnterReturnsTrue) )
	    			{
	    				unlink( it->second->GetMaterialName() );
		    			strncpy((char*)it->second->GetMaterialName(), namebuff, MATERIAL_MAX_NAME_LENGTH-1);
		    			it->second->OnIsDirty();
		    			doRefreshResources = true;
						SYSTEMS::GetObject().RESOURCES.OnRenderersDirty();
	    			}
		        	ImGui::TreePop();		    		
		    	}
				it->second->OnGUI();
		        ImGui::TreePop();
		    }

			//ImGui::Spacing();
        	ImGui::Separator();
		}

		// if(doRefreshResources)
		//     RefreshResources();
	}
	void ResourceSystem::RegisterRendererComponent(RendererComponent* obj)
	{
		v_rendererComponentsOnScene.push_back(obj);
	}
	void ResourceSystem::UnRegisterRendererComponent(RendererComponent* obj)
	{
		for(auto it = v_rendererComponentsOnScene.begin(); it!=v_rendererComponentsOnScene.end(); ++it)
		{
			if( *it==obj )
			{
				v_rendererComponentsOnScene.erase(it);
				break;
			}
		}
	}
	void ResourceSystem::OnRenderersDirty()
	{
		for(int i=0; i<v_rendererComponentsOnScene.size(); ++i)
		{
			v_rendererComponentsOnScene[i]->OnIsDirty();
		}
	}
	#endif

}