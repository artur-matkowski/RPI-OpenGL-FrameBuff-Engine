#include "EditorSystem.hpp"
#ifdef IS_EDITOR
#include "StatsWindow.hpp"
#include "HierarchyWindow.hpp"
#include "ComponentEditorWindow.hpp"
#include "imgui.h"      // IMGUI_IMPL_API

#include "Systems.hpp"

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
       	ImGui::NewFrame();

        ImGui::ShowDemoWindow(0);

		StatsWindow::OnGUI();

		HierarchyWindow::OnGUI();

		ComponentEditorWindow::OnGUI();
	}	
}

#endif