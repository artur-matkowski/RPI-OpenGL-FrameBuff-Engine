#include "HierarchyWindow.hpp"
#include "imgui.h"
#include "Systems.hpp"


namespace asapgl
{
	HierarchyWindow::HierarchyWindow( bfu::MemBlockBase* mBlock )
		:m_mBlock(mBlock)
		,v_SelectedGameObjects(mBlock)
	{
		v_SelectedGameObjects.reserve(128);
	}

	HierarchyWindow::~HierarchyWindow()
	{

	}


	bool HierarchyWindow::IsNodeSelected(GameObject* obj)
	{
		const int size = v_SelectedGameObjects.size();

		for(int i=0; i<size; ++i)
		{
			if(v_SelectedGameObjects[i] == obj)
				return true;
		}

		return false;
	}
	void HierarchyWindow::DeselectNode(GameObject* obj)
	{
		for(auto it = v_SelectedGameObjects.begin();
				it != v_SelectedGameObjects.end();
				++it)
		{
			if(*it == obj){
				v_SelectedGameObjects.erase(it);
				return;
			}
		}
	}


	void HierarchyWindow::OnGUInode(GameObject* obj)
	{
        const ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;


		const int size = obj->v_children.size();
        for (int i = 0; i < size; ++i)
        {
            // Disable the default "open on single-click behavior" + set Selected flag according to our selection.
            ImGuiTreeNodeFlags node_flags = base_flags;

            const bool isSelected = IsNodeSelected(obj->v_children[i]);
            if ( isSelected )
                node_flags |= ImGuiTreeNodeFlags_Selected;
            
            bool node_open = ImGui::TreeNodeEx((void*) obj->v_children[i] , node_flags,  obj->v_children[i]->m_myName.c_str() );
            if ( ImGui::IsItemClicked() ){
            	if( !ImGui::GetIO().KeyCtrl )
            		v_SelectedGameObjects.clear();

            	if( isSelected )
            		DeselectNode( obj->v_children[i] );
            	else
                	v_SelectedGameObjects.push_back( obj->v_children[i] );
            }
            if (node_open)
            {
                OnGUInode(obj->v_children[i]);
                ImGui::TreePop();
            }
            
        }
	}

	void HierarchyWindow::OnGUI()
	{
		static GameObject* go_root = &SYSTEMS::GetObject().SCENE.GetRootNode();
		
		ImGui::Begin("Scene Hierarhy"); 

        OnGUInode( go_root );

	    ImGui::End();
	}
}