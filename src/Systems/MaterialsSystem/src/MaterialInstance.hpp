#ifndef H_MaterialInstance
#define H_MaterialInstance
#include <vector>
#include "UniqueID.hpp"
#include "_ResourceProcessorsInclude.hpp"

using bfu::string;


#define MATERIAL_ASSET_EXTENSION ".mat"
#define MATERIAL_DATA_EXTENSION ".mat.json"

namespace asapi
{
	using bfu::string;

	class MaterialInstance: public SerializableObject<MaterialInstance>
	{
		bool 											m_isDirty = true; //on init shader need to be compiled and processed
		SERIALIZABLE_OBJ(MaterialInstance, ResourceGLSLSharedReference, 
														m_shader );

		SERIALIZABLE_OBJ(MaterialInstance, UniqueID, 	m_uuid);

		static std::string 								s_projectPath;


		//std::vector<UniformInterface> v_uniforms;
		//ResourceGLSLProcessor			m_shader;

		static void OnShaderDirtyCallback(void* data);

	public:
		MaterialInstance(const UniqueID& uuid);
		~MaterialInstance();

		static void SetProjectPath(const std::string& path);

		#ifdef IS_EDITOR
		void OnGUI_SelectShader();
		#endif
	};
}

#endif