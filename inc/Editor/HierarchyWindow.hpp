#ifndef _H_HierarchyWindow
#define _H_HierarchyWindow
#include "GameObject.hpp"
#include <bitforge/utils/bfu.hpp>	

namespace asapgl
{
	class HierarchyWindow
	{
		bfu::MemBlockBase* 				m_mBlock = 0;

		std::vector<GameObject*, bfu::custom_allocator<GameObject*> > 
										v_SelectedGameObjects;
	public:
		HierarchyWindow( bfu::MemBlockBase* mBlock );
		~HierarchyWindow();

		bool IsNodeSelected(GameObject*);
		void DeselectNode(GameObject*);
		void OnGUInode(GameObject* obj);
		void OnGUI();		
	};
}

#endif