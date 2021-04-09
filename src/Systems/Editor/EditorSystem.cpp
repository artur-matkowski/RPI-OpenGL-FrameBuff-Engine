#include "EditorSystem.hpp"
#ifdef IS_EDITOR
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

	void EditorSystem::OnGUI()
	{
		StatsWindow::OnGUI();

		HierarchyWindow::OnGUI();

		ComponentEditorWindow::OnGUI();
	}	
}

#endif