#ifndef _H_HierarchyWindow
#define _H_HierarchyWindow
#include "GameObject.hpp"
#include <bitforge/utils/bfu.hpp>	

namespace asapi
{
	class HierarchyWindow
	{
		std::vector<GameObject*, bfu::custom_allocator<GameObject*> > 
										v_SelectedGameObjects;


		bool IsNodeSelected(GameObject*);
		void DeselectNode(GameObject*);
		void OnGUInode(GameObject* obj);

		HierarchyWindow();
		~HierarchyWindow();
	public:

		static void OnGUI();		
	};
}

#endif