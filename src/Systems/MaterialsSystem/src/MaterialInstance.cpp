#include "MaterialInstance.hpp"
#include "imgui.h"

namespace asapi
{
	std::string MaterialInstance::s_projectPath;

	MaterialInstance::MaterialInstance(const UniqueID& uuid)
	{
		FILE::MMAP materialDataFile;
		bfu::JSONSerializer serializer;

		m_uuid = uuid;
		m_shader.BindOnDirtyCallback(OnShaderDirtyCallback, this);

		std::string materialDataPath = s_projectPath + RESOURCE_BINARIES_DIR "/";
		materialDataPath += std::to_string( uuid.ID() ) + MATERIAL_DATA_EXTENSION;


		materialDataFile.InitForRead( materialDataPath.c_str() );

		log::debug << "Opening MaterialInstance (data) " << materialDataPath.c_str() << std::endl;

		serializer.assignData( (char*)materialDataFile.Data(), materialDataFile.Size() );
		serializer.Deserialize( this );
	}

	MaterialInstance::~MaterialInstance()
	{}
	
	void MaterialInstance::SetProjectPath(const std::string& path)
	{
		s_projectPath = path;
	}
	
	void MaterialInstance::OnShaderDirtyCallback(void* data)
	{
		log::debug << "MaterialInstance::OnShaderDirtyCallback" << std::endl;
	}

	#ifdef IS_EDITOR
	void MaterialInstance::OnGUI_SelectShader()
	{
		if (ImGui::BeginCombo("Shader resource", "m_shaderName"))
        {
            m_shader.OnGUI_caller();
            ImGui::EndCombo();
        }
	}
	#endif
}