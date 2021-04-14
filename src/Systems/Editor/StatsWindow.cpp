#include "StatsWindow.hpp"
#ifdef IS_EDITOR
#include "imgui.h"
#include "ImGuiFileDialog.h"
#include "PrefabLoaderComponent.hpp"

namespace asapi
{
	Persistance StatsWindow::persistance;
	ImGuiFileDialog instance;

	#ifdef IS_EDITOR

	void StatsWindow::drawFileGui(const std::string& dummyDlgKey)
	{ 
		// display
		if (instance.Display(dummyDlgKey)) 
		{
			// action if OK
			if (instance.IsOk() )
			{
				if( OpenProject( instance.GetCurrentPath().c_str() ) )
					persistance.Update( instance.GetCurrentPath().c_str() );
				// action
			}

			// close
			instance.Close();
		}
	}


	void StatsWindow::OnGUI()
	{
		auto window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse ;
		static bool showFileWindow = false;
		static std::string dummyDlgKey = "ChooseFileDlgKey";
		MemoryManagmentSystem& MEMORY = SYSTEMS::GetObject().MEMORY;
		SceneSystem& SCENE = SYSTEMS::GetObject().SCENE;

		ImGui::Begin("Editor System", NULL, window_flags ); 



		// Menu Bar
	    if (ImGui::BeginMenuBar())
	    {
	        if (ImGui::BeginMenu("Menu"))
	        {
			    if (ImGui::MenuItem("Open Project...")) { showFileWindow = true; }
			    if (ImGui::BeginMenu("Open Recent"))
		        {
				    if ( ImGui::MenuItem(persistance.m_lastOpenProject0.c_str()) ) { OpenProject( persistance.m_lastOpenProject0.c_str() ); }
				    if ( ImGui::MenuItem(persistance.m_lastOpenProject1.c_str()) ) { OpenProject( persistance.m_lastOpenProject1.c_str() ); }
				    if ( ImGui::MenuItem(persistance.m_lastOpenProject2.c_str()) ) { OpenProject( persistance.m_lastOpenProject2.c_str() ); }
				    if ( ImGui::MenuItem(persistance.m_lastOpenProject3.c_str()) ) { OpenProject( persistance.m_lastOpenProject3.c_str() ); }
	           		ImGui::EndMenu();
		        }

			    if (ImGui::MenuItem("Save Project...")) 
			    { 
			    	PrefabLoaderComponent* cmp = (PrefabLoaderComponent*) SCENE.GetRootNode().GET_COMPONENT(PrefabLoaderComponent);
			    	cmp->Save_JSON(); 
			    }
			    ImGui::Separator();

			    if (ImGui::MenuItem("Compile to .mmp")) {}
			    if (ImGui::MenuItem("Load from .mmp")) {}
			    ImGui::Separator();

			    if (ImGui::MenuItem("Quit")) { SYSTEMS::GetObject().cloaseApp(); }
	            ImGui::EndMenu();
	        }
	        ImGui::EndMenuBar();
	    }
			
		if(showFileWindow)
		{
			instance.OpenDialog(dummyDlgKey, "Choose File", nullptr, ".");
			showFileWindow = false;
		}
		drawFileGui(dummyDlgKey);

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
                ImGui::LabelText("Currently opened project", SYSTEMS::GetObject().SCENE.GetProjectPath());
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

	    ImGui::End();
	}
	#endif


	bool StatsWindow::OpenProject(const char* path)
	{
		SYSTEMS::GetObject().SCENE.SetProjectPath(path);

		if ( SYSTEMS::GetObject().SCENE.OpenProject() )
		{
            ImGui::OpenPopup("CanNotOpen");
		}

        // Always center this window when appearing
        ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("CanNotOpen", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Can not open project at path: \n%s!\n\n", path);
            ImGui::Separator();

            if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::SetItemDefaultFocus();

            ImGui::EndPopup();
        }
        return true;
	}
}

#endif