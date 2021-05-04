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

		UniformBase**								p_uniforms = nullptr;

	public:
		MaterialType(const char*, bfu::MemBlockBase* materialsMemBlock, bfu::MemBlockBase* metadataMemBlock);
		~MaterialType();

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

			std::map<std::string, UniformBase*>::iterator it( m_uniformMap.begin() );
			for(; it != m_uniformMap.end(); ++it)
			{
				it->second->SendUniform();
			}
			
		}

		#ifdef IS_EDITOR
		void OnGUI();
		#endif
	};
}

#endif