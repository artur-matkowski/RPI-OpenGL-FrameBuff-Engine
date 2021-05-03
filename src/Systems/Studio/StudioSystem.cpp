#include "StudioSystem.hpp"
#ifdef IS_EDITOR
#include "StatsWindow.hpp"
#include "HierarchyWindow.hpp"
#include "EditorWindow.hpp"
#include "imgui.h"      // IMGUI_IMPL_API

#include "Systems.hpp"

namespace asapi
{

	StudioSystem::StudioSystem()
	{

	}

	StudioSystem::~StudioSystem()
	{

	}

	void StudioSystem::Init(const int argc, const char** argv)
	{
		StatsWindow::Init();
		char buff[MAX_PATH_SIZE];

		for(int i=1; i<argc; ++i)
		{
			if( (strcmp(argv[i], "-compile") == 0) && argc>i )
			{
				if(argv[i+1][0]!='/')
				{
					strncpy(buff, get_current_dir_name(), MAX_PATH_SIZE-1);
					strncat(buff, "/", MAX_PATH_SIZE-1);
					strncat(buff, argv[i+1], MAX_PATH_SIZE-1);
				}
				else
				{
					strncpy(buff, argv[i+1], MAX_PATH_SIZE);
				}
				SYSTEMS::GetObject().RESOURCES.SetProjectPath(buff);
				SYSTEMS::GetObject().RESOURCES.RefreshResources();
				SYSTEMS::GetObject().cloaseApp();
			}
		}
	}


	void StudioSystem::OnGUI()
	{
       	ImGui::NewFrame();

        ImGui::ShowDemoWindow(0);

		StatsWindow::OnGUI();

		HierarchyWindow::OnGUI();

		EditorWindow::OnGUI();
	}	
}

#endif