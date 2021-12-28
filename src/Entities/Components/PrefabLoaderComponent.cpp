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
		//ImGui::LabelText("Prefab ID", "%lld", m_prefabID.GetRef().ID() );

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

	void PrefabLoaderComponent::OnAttach()
	{
	}

	bfu::MemBlockBase* PrefabLoaderComponent::RequestPrefabMemBlock()
	{
		if(m_prefabMemBlock!=0) return m_prefabMemBlock;

		char _buff[256];
		bfu::stream buff(_buff, 256 );

		buff.clear();
		buff.sprintf( "prefab allocator: %lld"
						, m_prefabID.ID() );

		m_prefabMemBlock = SYSTEMS::GetObject().MEMORY.RequestPrefabMemBlock(buff.c_str());

		return m_prefabMemBlock;
	}

	void PrefabLoaderComponent::Save_JSON()
	{
		char _buff[256];
		bfu::stream buff(_buff, 256 );

		buff.clear();
		buff.sprintf( "%s/json/%lld.json"
						, SYSTEMS::GetObject().ASSETS.GetProjectPath()
						, GetPrefabID() );

		bfu::JSONSerializer &jsonSerializer = SYSTEMS::GetObject().SCENE.GetJSONSerializer();
		jsonSerializer.clear();

		m_owner->SerializeChildren( jsonSerializer );

		SceneSystem::JSON2File( jsonSerializer, buff.c_str() ) ;
	}
	bool PrefabLoaderComponent::Load_JSON()
	{
		char _buff[256];
		bfu::stream buff(_buff, 256 );

		buff.clear();
		buff.sprintf( "prefab allocator: %lld"
						, m_prefabID.ID() );

		m_owner->ClearChildren();

		if(m_prefabMemBlock!=0) SYSTEMS::GetObject().MEMORY.ReleasePrefabMemBlock(m_prefabMemBlock);

		m_prefabMemBlock = SYSTEMS::GetObject().MEMORY.RequestPrefabMemBlock(buff.c_str());

		buff.clear();
		buff.sprintf( "%s/json/%lld.json"
						, SYSTEMS::GetObject().ASSETS.GetProjectPath()
						, GetPrefabID() );

		
		bfu::JSONSerializer &jsonSerializer = SYSTEMS::GetObject().SCENE.GetJSONSerializer();
		jsonSerializer.clear();

		if( SceneSystem::File2JSON( jsonSerializer, buff.c_str() ) )
		{
			m_owner->DeserializeChildren( jsonSerializer, m_prefabMemBlock );
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
		SYSTEMS::GetObject().MEMORY.ReleasePrefabMemBlock( m_prefabMemBlock );
	}

	void PrefabLoaderComponent::SetPrefabID(uint64_t id)
	{
		m_prefabID.SetID(id);
	}
	uint64_t PrefabLoaderComponent::GetPrefabID()
	{
		return m_prefabID.ID();
	}
}