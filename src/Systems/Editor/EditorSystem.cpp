#include "EditorSystem.hpp"
#ifdef IS_EDITOR
#include "imgui.h"      // IMGUI_IMPL_API
#include "StatsWindow.hpp"
#include "HierarchyWindow.hpp"
#include "ComponentEditorWindow.hpp"

namespace asapi
{

	EditorSystem::EditorSystem()
	{

	}

	EditorSystem::~EditorSystem()
	{

	}

	void EditorSystem::Init()
	{
		StatsWindow::Init();
	}


	void EditorSystem::OnGUI()
	{
        ImGui::ShowDemoWindow(0);

		StatsWindow::OnGUI();

		HierarchyWindow::OnGUI();

		ComponentEditorWindow::OnGUI();
	}	
}

#endif