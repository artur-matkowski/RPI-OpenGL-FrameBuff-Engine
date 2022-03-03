#include "MaterialInstance.hpp"

namespace asapi
{
	std::string MaterialInstance::s_projectPath;

	MaterialInstance::MaterialInstance(const UniqueID& uuid)
	{
		std::string materialDataPath = s_projectPath + "/";
		materialDataPath += std::to_string( uuid.ID() ) + ".mat.json";

		log::debug << "Opening MaterialInstance (data) " << materialDataPath.c_str() << std::endl;
	}
	
	void MaterialInstance::SetProjectPath(const char* path)
	{
		s_projectPath = path;
	}
}