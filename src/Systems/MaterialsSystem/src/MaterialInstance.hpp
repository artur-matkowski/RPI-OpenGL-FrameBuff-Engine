#ifndef H_MaterialInstance
#define H_MaterialInstance
#include <vector>
#include "UniqueID.hpp"

using bfu::string;

namespace asapi
{
	class MaterialInstance: public SerializableObject<MaterialInstance>
	{
		SERIALIZABLE_OBJ(MaterialInstance, UniqueID, 	m_uuid);
		SERIALIZABLE_OBJ(MaterialInstance, UniqueID, 	m_shaderID);

		static std::string 								s_projectPath;

		//std::vector<UniformInterface> v_uniforms;

		//ResourceGLSLProcessor			m_shader;

	public:
		MaterialInstance(const UniqueID& uuid);
		~MaterialInstance();

		static void SetProjectPath(const std::string& path);

	};
}

#endif