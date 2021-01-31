#include "EditorSystem.hpp"
#ifndef _PLAYER

namespace asapgl
{

	EditorSystem::EditorSystem( bfu::MemBlockBase* mBlock )
		:m_HierarchyWindow(mBlock)
	{

	}

	EditorSystem::~EditorSystem()
	{

	}

	void EditorSystem::OnGUI()
	{
		m_HierarchyWindow.OnGUI();
	}	
}

#endif