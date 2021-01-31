#ifndef _H_EditorSystem
#define _H_EditorSystem
#ifndef _PLAYER
#include "HierarchyWindow.hpp"

namespace asapgl
{
	class EditorSystem
	{
		HierarchyWindow		m_HierarchyWindow;
		
	public:
		EditorSystem( bfu::MemBlockBase* mBlock );
		~EditorSystem();

		void OnGUI();
		
	};
}
#endif
#endif