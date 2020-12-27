#ifndef _H_ResourceManager
#define _H_ResourceManager

#include <map>
#include "ResourcePtr.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "Mesh.hpp"

namespace asapgl
{

	class ResourceSystem
	{
		std::map<std::string, ResourcePtr<Texture> > 	m_textures;
		std::map<std::string, ResourcePtr<Shader> > 	m_shaders;
		std::map<std::string, ResourcePtr<Material> > 	m_materials;
		std::map<std::string, ResourcePtr<Mesh> > 		m_meshes;

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

		bool requestResource(ResourcePtr<Material>* res, const char* str)
		{
			std::string id(str);

			std::map<std::string, ResourcePtr<Material> >::iterator it = m_materials.find(id);

			if( it==m_materials.end() )
			{
				res->Rebuild( new Material(str) );
				m_materials[id] = *res;
			}
			else
			{
				*res = m_materials[id];
			}
			
			return true;
		}

		void dispouseResource(ResourcePtr<Material>* res)
		{
			for(std::map<std::string, ResourcePtr<Material> >::iterator it = m_materials.begin() ;
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
			std::string id(str);

			std::map<std::string, ResourcePtr<Mesh> >::iterator it = m_meshes.find(id);

			if( it==m_meshes.end() )
			{
				res->Rebuild( new Mesh(str) );
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
			for(std::map<std::string, ResourcePtr<Mesh> >::iterator it = m_meshes.begin() ;
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