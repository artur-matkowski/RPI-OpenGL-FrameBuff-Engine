#include "PrefabLoaderComponent.hpp"
#include "imgui.h"
#include "Systems.hpp"

namespace asapi
{
	void PrefabLoaderComponent::OnGUI()
	{
		ImGui::LabelText("Prefab ID", "%lld", m_prefabID.GetRef().ID() );
		if( ImGui::Button("Load") ) Load();
		ImGui::SameLine();
		ImGui::PushItemWidth(-ImGui::GetContentRegionAvail().x * 0.5f);
		ImGui::Button("Unload"); 
		ImGui::SameLine();
		ImGui::PushItemWidth(-100.0f);
		if( ImGui::Button("Save") ) Save();
	}


	void PrefabLoaderComponent::Save()
	{
		char buff[2048];
		bfu::stream path(buff, 2048, SYSTEMS::STD_ALLOCATOR );

		path.sprintf( "%s/json/%lld.json"
						, SYSTEMS::GetObject().SCENE.GetProjectPath()
						, GetPrefabID() );

		bfu::JSONStream& jsonStream = SYSTEMS::GetObject().SCENE.GetJSONStreamWorkBuffer();
		jsonStream.clear();

		m_owner->SerializeChildren( jsonStream );
		
		SceneSystem::JSON2File( jsonStream, path.c_str() ) ;
	}
	void PrefabLoaderComponent::Load()
	{
		char buff[2048];
		bfu::stream path(buff, 2048, SYSTEMS::STD_ALLOCATOR );

		path.sprintf( "%s/json/%lld.json"
						, SYSTEMS::GetObject().SCENE.GetProjectPath()
						, GetPrefabID() );

		bfu::JSONStream& jsonStream = SYSTEMS::GetObject().SCENE.GetJSONStreamWorkBuffer();
		jsonStream.clear();

		if( SceneSystem::File2JSON( jsonStream, path.c_str() ) )
		{
			m_owner->DeserializeChildren( jsonStream );
		}
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