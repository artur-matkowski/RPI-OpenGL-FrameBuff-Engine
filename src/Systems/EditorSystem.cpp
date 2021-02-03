#include "EditorSystem.hpp"
#ifdef IS_EDITOR
#include "imgui.h"
#include "Systems.hpp"

namespace asapgl
{

	EditorSystem::EditorSystem( bfu::MemBlockBase* mBlock )
		:m_HierarchyWindow(mBlock)
	{

	}

	EditorSystem::~EditorSystem()
	{

	}

	void EditorSystem::OnGUI()
	{
		auto window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;
		MemoryManagmentSystem& MEMORY = SYSTEMS::GetObject().MEMORY;

		ImGui::Begin("Editor System", NULL, window_flags ); 



		// Menu Bar
	    if (ImGui::BeginMenuBar())
	    {
	        if (ImGui::BeginMenu("Menu"))
	        {
			    if (ImGui::MenuItem("Serialize to JSON")) {}
			    if (ImGui::MenuItem("Deserialize from JSON")) {}
			    ImGui::Separator();

			    if (ImGui::MenuItem("Compile to .mmp")) {}
			    if (ImGui::MenuItem("Load from .mmp")) {}
			    ImGui::Separator();

			    if (ImGui::MenuItem("Quit")) { SYSTEMS::GetObject().cloaseApp(); }
	            ImGui::EndMenu();
	        }
	        ImGui::EndMenuBar();
	    }

	    MEMORY.OnGUI();

	    ImGui::End();

		m_HierarchyWindow.OnGUI();
	}	
}

#endif