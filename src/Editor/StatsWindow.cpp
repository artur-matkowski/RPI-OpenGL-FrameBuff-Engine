#include "StatsWindow.hpp"
#include "imgui.h"
#include "Systems.hpp"


namespace asapi
{
	char StatsWindow::m_openedProjectPath[2048] = "./";

	#ifdef IS_EDITOR
	void StatsWindow::OnGUI()
	{
		auto window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;
		static char 	openedProjectPath[2048] = "./";
		MemoryManagmentSystem& MEMORY = SYSTEMS::GetObject().MEMORY;
		SceneSystem& SCENE = SYSTEMS::GetObject().SCENE;

		ImGui::Begin("Editor System", NULL, window_flags ); 



		// Menu Bar
	    if (ImGui::BeginMenuBar())
	    {
	        if (ImGui::BeginMenu("Menu"))
	        {
			    if (ImGui::MenuItem("Serialize to JSON")) { SCENE.GetRootNode().Serialize(); }
			    if (ImGui::MenuItem("Deserialize from JSON")) { SCENE.GetRootNode().Deserialize(); }
			    ImGui::Separator();

			    if (ImGui::MenuItem("Compile to .mmp")) {}
			    if (ImGui::MenuItem("Load from .mmp")) {}
			    ImGui::Separator();

			    if (ImGui::MenuItem("Quit")) { SYSTEMS::GetObject().cloaseApp(); }
	            ImGui::EndMenu();
	        }
	        ImGui::EndMenuBar();
	    }


	    //Tabs
	    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("ProjectInfoTabs", tab_bar_flags))
        {
            if (ImGui::BeginTabItem("Memory allocators info"))
            {
                MEMORY.OnGUI();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Opened project info"))
            {
                ImGui::LabelText("Currently opened project", openedProjectPath);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

	    ImGui::End();
	}
	#endif
}