#ifndef _H_ResourceManager
#define _H_ResourceManager

#include <map>
#include "ResourcePtr.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

namespace asapgl
{

	class ResourceSystem
	{
		std::map<std::string, ResourcePtr<Texture> > 	m_textures;
		std::map<std::string, ResourcePtr<Shader> > 	m_shaders;

	public:

		bool requestResource(ResourcePtr<Texture>* res, const char* str)
		{
			std::string id(str);

			std::map<std::string, ResourcePtr<Texture> >::iterator it = m_textures.find(id);

			if( it==m_textures.end() )
			{
				res->Rebuild( new Texture(str) );
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
			for(std::map<std::string, ResourcePtr<Texture> >::iterator it = m_textures.begin() ;
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
			std::string id(str);

			std::map<std::string, ResourcePtr<Shader> >::iterator it = m_shaders.find(id);

			if( it==m_shaders.end() )
			{
				res->Rebuild( new Shader(str) );
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
			for(std::map<std::string, ResourcePtr<Shader> >::iterator it = m_shaders.begin() ;
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
	};

}


#endif