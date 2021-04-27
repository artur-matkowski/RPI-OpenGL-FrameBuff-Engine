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

	void StudioSystem::Init()
	{
		StatsWindow::Init();
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