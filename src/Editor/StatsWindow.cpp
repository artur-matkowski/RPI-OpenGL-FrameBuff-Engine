#include "StatsWindow.hpp"
#include "imgui.h"
#include "ImGuiFileDialog.h"
#include "PrefabLoaderComponent.hpp"

namespace asapi
{
	StatsWindow::Persistance* StatsWindow::persistance;
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
					persistance->Update( instance.GetCurrentPath().c_str() );
				// action
			}

			// close
			instance.Close();
		}
	}


	void StatsWindow::OnGUI()
	{
		auto window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;
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
				    if ( ImGui::MenuItem(persistance->m_lastOpenProject0.GetRef().c_str()) ) { OpenProject( persistance->m_lastOpenProject0.GetRef().c_str() ); }
				    if ( ImGui::MenuItem(persistance->m_lastOpenProject1.GetRef().c_str()) ) { OpenProject( persistance->m_lastOpenProject1.GetRef().c_str() ); }
				    if ( ImGui::MenuItem(persistance->m_lastOpenProject2.GetRef().c_str()) ) { OpenProject( persistance->m_lastOpenProject2.GetRef().c_str() ); }
				    if ( ImGui::MenuItem(persistance->m_lastOpenProject3.GetRef().c_str()) ) { OpenProject( persistance->m_lastOpenProject3.GetRef().c_str() ); }
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
    	PrefabLoaderComponent* cmp = (PrefabLoaderComponent*) SYSTEMS::GetObject().SCENE.GetRootNode().GET_COMPONENT(PrefabLoaderComponent);
    	cmp->Load_JSON(); 

		return true;
	}
}