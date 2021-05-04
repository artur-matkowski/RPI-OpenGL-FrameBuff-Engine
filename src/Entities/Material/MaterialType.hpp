#ifndef _H_MaterialType
#define _H_MaterialType
#include "ResourcePtr.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "_UniformsList.hpp"
#include "object.hpp"
#include <map>


namespace asapi
{
	class MaterialType: public object
	{
		//SERIALIZABLE_VAR( std::string ) 			m_usedShader;
		//SERIALIZABLE_VAR_VEC( std::string )		 	m_usedTextures;

		ResourcePtr< Shader > 						m_shader;

		int32_t										m_uniformsCount;											
		UniformInterface**								p_uniforms = nullptr;

	public:
		MaterialType(const char*, bfu::MemBlockBase* materialsMemBlock, bfu::MemBlockBase* metadataMemBlock);
		~MaterialType();

		inline UniformInterface* GetUniformPtr(const char* uniformName)
		{
			UniformInterface* ret = nullptr;

			for(int i=0; i<m_uniformsCount; ++i)
			{
				if( p_uniforms[i]->Is(uniformName))
				{
					ret = p_uniforms[i];
					break;
				}
			}

			#ifdef IS_EDITOR
			if(ret==nullptr)
			{
				log::warning << "Could not find uniform " << uniformName << std::endl;
			}
			#endif

			return ret;
		}


		inline void BindMaterial()
		{
			m_shader->UseProgram();

			for(int i=0; i<m_uniformsCount; ++i)
			{
				p_uniforms[i]->SendUniform();
			}
		}

		#ifdef IS_EDITOR
		void OnGUI();
		#endif
	};
}

#endif