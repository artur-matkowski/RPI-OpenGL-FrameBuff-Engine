#ifndef _H_ResourceManager
#define _H_ResourceManager

#include <map>
#include "ResourcePtr.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "MaterialType.hpp"
#include "Mesh.hpp"
#include "AssetMetaDataSocket.hpp"

namespace asapi
{

	class ResourceSystem
	{
		bfu::MemBlockBase* 									p_materialsMemBlock;

		std::map<bfu::string, ResourcePtr<Texture> > 		m_textures;
		std::map<bfu::string, ResourcePtr<Shader> > 		m_shaders;
		std::map<bfu::string, ResourcePtr<MaterialType> > 	m_materials;
		std::map<bfu::string, ResourcePtr<Mesh> > 			m_meshes;

		char 												m_ProjectPath[MAX_PATH_SIZE] = ".";

		#ifdef IS_EDITOR
		std::map<std::string, AssetMetaDataSocket>			m_assetsMap;

		std::vector<std::string>							v_TexturesPaths;
		std::vector<std::string>							v_MaterialsPaths;
		std::vector<std::string>							v_ShadersPaths;
		std::vector<std::string>							v_MeshesPaths;

		std::vector<RendererComponent*>						v_rendererComponentsOnScene;

		void BuildDirectoryStructure();
		#endif

	public:
		void Init(const int argc, const char** argv);
		void SetProjectPath(const char* path);
		const char* GetProjectPath(){ return m_ProjectPath; }

		#ifdef IS_EDITOR
		void RefreshAssets();


		void RefreshResources();
		std::vector<std::string>* GetTexturesPaths(){ return &v_TexturesPaths; }
		std::vector<std::string>* GetMaterialsPaths(){ return &v_MaterialsPaths; }
		std::vector<std::string>* GetShadersPaths(){ return &v_ShadersPaths; }
		std::vector<std::string>* GetMeshesPaths(){ return &v_MeshesPaths; }
		void OnGUI();
		void RegisterRendererComponent(RendererComponent*);
		void UnRegisterRendererComponent(RendererComponent*);
		void OnRenderersDirty();
		#endif

		bool requestResource(ResourcePtr<Texture>* res, const char* str)
		{
			bfu::string id(str);

			std::map<bfu::string, ResourcePtr<Texture> >::iterator it = m_textures.find(id);

			if( it==m_textures.end() )
			{
				char buff[MAX_PATH_SIZE];
				snprintf(buff, MAX_PATH_SIZE, "%s/assets_int/textures/%s", m_ProjectPath, str);
				res->Rebuild( new Texture(buff) );
				m_textures[id] = *res;
			}
			else
			{
				*res = m_textures[id];
			}
			
			return true;
		}

		void dispouseResource(ResourcePtr<Texture>* res)
		{
			for(std::map<bfu::string, ResourcePtr<Texture> >::iterator it = m_textures.begin() ;
				it!=m_textures.end();
				++it)
			{
				void* A = it->second.GetRawPtr();
				void* B = res->GetRawPtr();

				if( A == B )
				{
					m_textures.erase(it);
					break;
				}
			}
		}

		bool requestResource(ResourcePtr<Shader>* res, const char* str)
		{
			bfu::string id(str);

			std::map<bfu::string, ResourcePtr<Shader> >::iterator it = m_shaders.find(id);

			if( it==m_shaders.end() )
			{
				Shader* newShader = Shader::LoadShaderFromFile(str);
				if( newShader==0 )
				{
					log::warning << "Could not load shader '" << str << "' reattemping with 'debug' shader" << std::endl;
					//already done in renderer system
				}
				res->Rebuild( newShader );
				m_shaders[id] = *res;
			}
			else
			{
				*res = m_shaders[id];
			}
			
			return true;
		}

		void dispouseResource(ResourcePtr<Shader>* res)
		{
			for(std::map<bfu::string, ResourcePtr<Shader> >::iterator it = m_shaders.begin() ;
				it!=m_shaders.end();
				++it)
			{
				void* A = it->second.GetRawPtr();
				void* B = res->GetRawPtr();

				if( A == B )
				{
					m_shaders.erase(it);
					break;
				}
			}
		}

		bool requestResource(ResourcePtr<MaterialType>* res, const char* str)
		{
			bfu::string id(str);

			std::map<bfu::string, ResourcePtr<MaterialType> >::iterator it = m_materials.find(id);

			if( it==m_materials.end() )
			{
				MaterialType* mat = (MaterialType*)p_materialsMemBlock->allocate(1, sizeof(MaterialType), alignof(MaterialType));
				new (mat) MaterialType(str);
				res->Rebuild( mat );
				m_materials[id] = *res;
			}
			else
			{
				*res = m_materials[id];
			}
			
			return (*res)->IsValid();
		}

		void dispouseResource(ResourcePtr<MaterialType>* res)
		{
			for(std::map<bfu::string, ResourcePtr<MaterialType> >::iterator it = m_materials.begin() ;
				it!=m_materials.end();
				++it)
			{
				void* A = it->second.GetRawPtr();
				void* B = res->GetRawPtr();

				if( A == B )
				{
					m_materials.erase(it);
					break;
				}
			}
		}

		bool requestResource(ResourcePtr<Mesh>* res, const char* str)
		{
			bfu::string id(str);

			std::map<bfu::string, ResourcePtr<Mesh> >::iterator it = m_meshes.find(id);

			if( it==m_meshes.end() )
			{
				char buff[MAX_PATH_SIZE];
				snprintf(buff, MAX_PATH_SIZE, "%s/assets_int/meshes/%s", m_ProjectPath, str);
				res->Rebuild( new Mesh(buff) );
				m_meshes[id] = *res;
			}
			else
			{
				*res = m_meshes[id];
			}
			
			return true;
		}

		void dispouseResource(ResourcePtr<Mesh>* res)
		{
			for(std::map<bfu::string, ResourcePtr<Mesh> >::iterator it = m_meshes.begin() ;
				it!=m_meshes.end();
				++it)
			{
				void* A = it->second.GetRawPtr();
				void* B = res->GetRawPtr();

				if( A == B )
				{
					m_meshes.erase(it);
					break;
				}
			}
		}
	};
}


#endif