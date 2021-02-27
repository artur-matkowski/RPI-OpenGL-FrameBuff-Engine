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
}