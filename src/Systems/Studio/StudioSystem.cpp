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

		for(int i=1; i<argc; ++i)
		{
			if( (strcmp(argv[i], "-compile") == 0) && argc>i )
			{
				SYSTEMS::GetObject().RESOURCES.SetProjectPath(argv[i+1]);
				SYSTEMS::GetObject().RESOURCES.RefreshResources();
				exit(0);
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