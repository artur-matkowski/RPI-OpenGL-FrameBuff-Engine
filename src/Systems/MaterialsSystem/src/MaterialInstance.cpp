#include "MaterialInstance.hpp"

namespace asapi
{
	std::string MaterialInstance::s_projectPath;

	MaterialInstance::MaterialInstance(const UniqueID& uuid)
	{
		m_uuid = uuid;
		m_shaderID = 0;

		std::string materialDataPath = s_projectPath + "/";
		materialDataPath += std::to_string( uuid.ID() ) + ".mat.json";

		log::debug << "Opening MaterialInstance (data) " << materialDataPath.c_str() << std::endl;
	}

	MaterialInstance::~MaterialInstance()
	{
		std::string materialDataPath = s_projectPath + "/";
		materialDataPath += std::to_string( m_uuid.ID() ) + ".mat.json";

		log::debug << "Destroying MaterialInstance (data) " << materialDataPath.c_str() << std::endl;
	}
	
	void MaterialInstance::SetProjectPath(const char* path)
	{
		s_projectPath = path;
	}
}