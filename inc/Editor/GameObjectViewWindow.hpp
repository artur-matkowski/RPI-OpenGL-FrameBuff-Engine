#ifndef H_GameObjectViewWindow
#define H_GameObjectViewWindow
#include <vector>
#include <map>
#include "FolderLikeSelfStructure.hpp"

#define MAX_COMPONENT_PATH_SIZE 1024

namespace asapi
{
	class GameObject;
	
	class GameObjectViewWindow
	{

		Node<size_t> m_rootNode;



		GameObjectViewWindow();
		~GameObjectViewWindow();
	public:
		static void SetSelectedGameObject( GameObject* );
		static void OnGUI();

	};
}

#endif