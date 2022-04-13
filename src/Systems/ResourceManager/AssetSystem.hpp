#ifndef _H_ResourceManager
#define _H_ResourceManager

#include <map>
#include "ResourcePtr.hpp"
#include "Texture.hpp"
#include "object.hpp"
#include "RendererComponent.hpp"

namespace asapi
{

	class AssetSystem
	{
		bfu::MemBlockBase* 									p_materialsMemBlock;

		std::map<bfu::string, ResourcePtr<Texture> > 		m_textures;

		char 												m_ProjectPath[MAX_PATH_SIZE] = ".";

		#ifdef IS_EDITOR

		std::vector<std::string>							v_TexturesPaths;

		std::vector<RendererComponent*>						v_rendererComponentsOnScene;

		void BuildDirectoryStructure();
		#endif

	public:
		void Init(const int argc, const char** argv);
		void SetProjectPath(const char* path);
		const char* GetProjectPath(){ return m_ProjectPath; }

		#ifdef IS_EDITOR


		void RefreshResources();
		std::vector<std::string>* GetTexturesPaths(){ return &v_TexturesPaths; }
		
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
	};
}


#endif