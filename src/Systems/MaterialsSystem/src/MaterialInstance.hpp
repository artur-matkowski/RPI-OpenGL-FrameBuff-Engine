#ifndef H_MaterialInstance
#define H_MaterialInstance
#include <vector>
#include "UniqueID.hpp"
#include "_ResourceProcessorsInclude.hpp"
#include "Shader.hpp"
#include "_UniformsList.hpp"

using bfu::string;


#define MATERIAL_ASSET_EXTENSION ".mat"
#define MATERIAL_DATA_EXTENSION ".mat.json"

namespace asapi
{
	using bfu::string;

	class MaterialInstance: public SerializableObject<MaterialInstance>
	{
		static std::string 								s_projectPath;


		bool 											m_isDirty = true; //on init shader need to be compiled and processed

		SERIALIZABLE_OBJ(MaterialInstance, UniqueID, 	m_uuid);
		SERIALIZABLE_OBJ(MaterialInstance, ResourceGLSLSharedReference, 
														m_shaderResource );
		Shader 											m_shader;
		int16_t											m_uniformsCount = 0;
		UniformInterface**								p_uniforms = nullptr;
		Uniform<glm::mat4>*								p_modelViewUniform = nullptr;


		static void OnShaderDirtyCallback(void* data);

	public:
		MaterialInstance(const UniqueID& uuid);
		~MaterialInstance();

		static void SetProjectPath(const std::string& path);

		inline UniformInterface* GetUniformPtr(const char* uniformName)
		{
			UniformInterface* ret = nullptr;

			for(int16_t i=0; i<m_uniformsCount; ++i)
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
			m_shader.UseProgram();

			for(int i=0; i<m_uniformsCount; ++i)
			{
				p_uniforms[i]->SendUniform();
			}
		}

		inline bool IsValid() { return m_shader.IsValid(); }
		Uniform<glm::mat4>* GetModelViewMatrix(){ return p_modelViewUniform; }

		#ifdef IS_EDITOR
		void OnGUI_SelectShader();
		virtual void OnGUI() override;
		#endif
	};
}

#endif