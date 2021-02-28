#ifndef H_ComponentEditorWindow
#define H_ComponentEditorWindow
#ifdef IS_EDITOR

#include <vector>
#include <map>
#include "FolderLikeSelfStructure.hpp"

#define MAX_COMPONENT_PATH_SIZE 1024

namespace asapi
{
	class GameObject;
	
	class ComponentEditorWindow
	{

		Node<size_t> m_rootNode;



		ComponentEditorWindow();
		~ComponentEditorWindow();
	public:
		static void SetSelectedGameObject( GameObject* );
		static void OnGUI();

	};
}

#endif
#endif