#include "PrefabLoaderComponent.hpp"
#ifdef IS_EDITOR
#include "imgui.h"
#endif
#include "Systems.hpp"

namespace asapi
{

	#ifdef IS_EDITOR
	void PrefabLoaderComponent::OnGUI()
	{
		ImGui::LabelText("Prefab ID", "%lld", m_prefabID.GetRef().ID() );

		if( ImGui::Button("Load from JSON") ) Load_JSON();
		ImGui::SameLine();
		if( ImGui::Button("Save to JSON") ) Save_JSON();
		ImGui::SameLine();
		if( ImGui::Button("Load from MMP") ) Load_MMP();
		ImGui::SameLine();
		if( ImGui::Button("Compile to MMP") ) Save_MMP();

		if( ImGui::Button("Unload") ) UnLoad();
	}
	#endif


	void PrefabLoaderComponent::Save_JSON()
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
	bool PrefabLoaderComponent::Load_JSON()
	{
		char buff[2048];
		bfu::stream path(buff, 2048, SYSTEMS::STD_ALLOCATOR );

		path.sprintf( "prefab allocator: %lld"
						, m_prefabID.GetRef().ID() );

		if(m_prefabMemBlock!=0) m_prefabMemBlock->ForceDispouse();

		m_prefabMemBlock = PrefabMemBlock::InitNoFile( path.c_str() , 1024*1024);

		path.clear();
		path.sprintf( "%s/json/%lld.json"
						, SYSTEMS::GetObject().SCENE.GetProjectPath()
						, GetPrefabID() );

		bfu::JSONStream& jsonStream = SYSTEMS::GetObject().SCENE.GetJSONStreamWorkBuffer();
		jsonStream.clear();

		m_owner->OverrideChildVector( m_prefabMemBlock->GetEntryVector(), m_prefabMemBlock );

		if( SceneSystem::File2JSON( jsonStream, path.c_str() ) )
		{
			m_owner->DeserializeChildren( jsonStream );
			return true;
		}
		return false;
	}
	void PrefabLoaderComponent::Save_MMP()
	{

	}

	void PrefabLoaderComponent::Load_MMP()
	{

	}

	void PrefabLoaderComponent::UnLoad()
	{
		m_owner->ClearChildren();
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