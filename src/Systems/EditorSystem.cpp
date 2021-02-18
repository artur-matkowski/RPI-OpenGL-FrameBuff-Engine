#include "EditorSystem.hpp"
#ifdef IS_EDITOR
#include "StatsWindow.hpp"
#include "HierarchyWindow.hpp"
#include "GameObjectViewWindow.hpp"

namespace asapi
{

	EditorSystem::EditorSystem()
	{

	}

	EditorSystem::~EditorSystem()
	{

	}

	void EditorSystem::OnGUI()
	{
		StatsWindow::OnGUI();

		HierarchyWindow::OnGUI();

		GameObjectViewWindow::OnGUI();
	}	
}

#endif