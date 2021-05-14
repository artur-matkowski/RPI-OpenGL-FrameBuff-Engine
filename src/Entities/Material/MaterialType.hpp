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
	#define MATERIAL_MAX_NAME_LENGTH 255

	class MaterialType: public object
	{
		ResourcePtr< Shader > 						m_shader;

		int32_t										m_uniformsCount = 0;											
		UniformInterface**							p_uniforms = nullptr;
		Uniform<glm::mat4>*							p_modelViewUniform = nullptr;

		#ifdef IS_EDITOR	
		char										m_shaderName[MATERIAL_MAX_NAME_LENGTH] = {'\0'};
		char										m_MaterialName[MATERIAL_MAX_NAME_LENGTH] = {'\0'};
		#endif

	public:
		MaterialType(const char*);
		~MaterialType();
		void LoadShader(const char*);
		Uniform<glm::mat4>* GetModelViewMatrix(){ return p_modelViewUniform; }

		inline bool IsValid(){ return p_modelViewUniform != nullptr; }

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
		void OnIsDirty();
		const char* GetShaderName(){ return m_shaderName; }
		const char* GetMaterialName(){ return m_MaterialName; }
		#endif

		static void Compile(const char* dest, const char* source);
	};
}

#endif