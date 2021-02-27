#include "PrefabLoaderComponent.hpp"
#include "imgui.h"

namespace asapi
{
	void PrefabLoaderComponent::OnGUI()
	{
		ImGui::LabelText("Prefab ID", "%lld", m_prefabID.GetRef().ID() );
		ImGui::Button("Load"); 
		ImGui::SameLine();
		ImGui::PushItemWidth(-ImGui::GetContentRegionAvail().x * 0.5f);
		ImGui::Button("Unload"); 
		ImGui::SameLine();
		ImGui::PushItemWidth(-100.0f);
		ImGui::Button("Save"); 
	}


	void PrefabLoaderComponent::Save()
	{
		// char filePath[GAMEOBJECT_MAX_NAME_LENGTH+sizeof(SERIALIZATION_FILE_EXT)+sizeof(SERIALIZATION_FOLDER)]; 
		// bfu::JSONStream& jsonStream = SYSTEMS::GetObject().SCENE.GetJSONStreamWorkBuffer();

		// //building a file name;
		// strcpy(filePath, SERIALIZATION_FOLDER );
		// strncpy(filePath+sizeof(SERIALIZATION_FOLDER)-1, m_myName.c_str(), GAMEOBJECT_MAX_NAME_LENGTH );
		// strncpy(filePath+sizeof(SERIALIZATION_FOLDER)-1+m_myName.size(), SERIALIZATION_FILE_EXT, sizeof(SERIALIZATION_FILE_EXT));

		// FILE * pFile = fopen (filePath,"wb");

		// if( pFile==NULL )
		// {
		// 	log::error << "Could not open file: " << filePath << std::endl;
		// 	return;
 	// 	}

 	// 	jsonStream << v_children;

		// fwrite(jsonStream.c_str(), 1, jsonStream.size(), pFile);

		// fclose (pFile);

		// jsonStream.clear();
	}
	void PrefabLoaderComponent::Load()
	{

	}
	void PrefabLoaderComponent::UnLoad()
	{

	}

	void PrefabLoaderComponent::SetPrefabID(uint64_t id)
	{
		m_prefabID.GetRef().SetID(id);
	}
	uint64_t PrefabLoaderComponent::GetPrefabID()
	{
		return m_prefabID.GetRef().ID();
	}
}