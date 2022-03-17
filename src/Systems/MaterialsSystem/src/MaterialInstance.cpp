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
		m_shaderResource.BindOnDirtyCallback(OnShaderDirtyCallback, this);

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
		MaterialInstance* _this = (MaterialInstance*)data;

		_this->m_shader.LoadShaderFromResourceRawHandle( _this->m_shaderResource.GetRawHandle() );
		log::debug << "MaterialInstance::OnShaderDirtyCallback" << std::endl;
	}

	#ifdef IS_EDITOR
	void MaterialInstance::OnGUI_SelectShader()
	{
		if (ImGui::BeginCombo("Shader resource", "m_shaderName"))
        {
            m_shaderResource.OnGUI_caller();
            ImGui::EndCombo();
        }
	}
	void MaterialInstance::OnGUI()
	{
		if( m_shader.IsValid() )
			ImGui::Text("Material Instance is valid.");
		else
		{
			const ImVec4 warningColor(1.0, 1.0, 0.0, 1.0);
			ImGui::TextColored(warningColor, "Material Instance is not valid.");
		}
	}
	#endif
}