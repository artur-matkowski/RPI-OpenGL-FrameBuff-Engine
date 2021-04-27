#ifndef H_EditorWindow
#define H_EditorWindow
#ifdef IS_EDITOR

#include <vector>
#include <map>
#include "FolderLikeSelfStructure.hpp"

#define MAX_COMPONENT_PATH_SIZE 1024

namespace asapi
{
	class GameObject;
	
	class EditorWindow
	{

		Node<size_t> m_rootNode;



		EditorWindow();
		~EditorWindow();

		static void OnGUI(ComponentInterface* obj);
		static void OnGUI(GameObject* obj);
	public:
		static void SetSelectedGameObject( GameObject* );
		static void OnGUI();

	};
}

#endif
#endif