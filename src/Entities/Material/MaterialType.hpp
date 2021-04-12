#ifndef _H_MaterialType
#define _H_MaterialType
#include "ResourcePtr.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "UniformBase.hpp"
#include "object.hpp"
#include <map>


namespace asapi
{
	class MaterialType: public object
	{
		//SERIALIZABLE_VAR( std::string ) 			m_usedShader;
		//SERIALIZABLE_VAR_VEC( std::string )		 	m_usedTextures;

		ResourcePtr< Shader > 						m_shader;

		std::map<std::string, UniformBase*>			m_uniformMap;

		std::vector< UniformBase* >					m_dirtyUniforms;
		bool 										m_isDirty = false;

	public:
		MaterialType(const char*);
		~MaterialType();		

		inline void SetUniformDirty(UniformBase* dirty)
		{
			m_dirtyUniforms.push_back( dirty );
			m_isDirty = true;
		}

		inline UniformBase* GetUniformPtr(const std::string& uniformName)
		{
			std::map<std::string, UniformBase*>::iterator it( m_uniformMap.find(uniformName) );
			if( it != m_uniformMap.end() )
			{
				return it->second;
			}
			else
			{
				log::warning << "Could not find uniform " << uniformName << std::endl;
				return 0;
			}
		}


		inline void BindMaterial()
		{
			m_shader->UseProgram();

			if(m_isDirty)
			{
				const int size = m_dirtyUniforms.size();
				for(int i=0; i < size; ++i)
				{
					m_dirtyUniforms[i]->SendUniform();
				}
				m_dirtyUniforms.clear();
				m_isDirty = false;
			}
		}

		#ifdef IS_EDITOR
		void OnGUI();
		#endif
	};
}

#endif