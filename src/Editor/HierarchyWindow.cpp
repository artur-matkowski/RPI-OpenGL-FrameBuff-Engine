#include "HierarchyWindow.hpp"
#include "imgui.h"
#include "Systems.hpp"
#include "GameObjectViewWindow.hpp"
#ifdef IS_EDITOR


namespace asapi
{
	HierarchyWindow::HierarchyWindow()
		:v_SelectedGameObjects( &SYSTEMS::GetObject().MEMORY.GetSystemsAllocator() )
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

	static bool is_GOContextMenuOpen = false;

	void AddGameObject(GameObject* parrent)
	{
		GameObject *pgo = SYSTEMS::ALLOCATE<GameObject>(1);
		pgo->Init( parrent->GetMemBlock() );
		pgo->OnAttach(parrent);
	}
	void RemoveGameObject(GameObject* obj)
	{
		obj->OnDetach();
		obj->Dispouse();
		SYSTEMS::DEALLOCATE<GameObject>(obj, 1);
	}

	void RenameGameObject(GameObject* obj)
	{
		static GameObject* ranemedGO = 0;
		static char buff[GAMEOBJECT_MAX_NAME_LENGTH];

		if( ranemedGO != obj )
		{
			ranemedGO = obj;
			strncpy(buff, obj->GetName(), GAMEOBJECT_MAX_NAME_LENGTH);
		}

	    ImGui::InputText("Rename GameObject", buff, GAMEOBJECT_MAX_NAME_LENGTH);

		obj->SetName(buff);
	}

	void HierarchyWindow::OnGUInode(GameObject* obj)
	{
        const ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | 
        										ImGuiTreeNodeFlags_OpenOnDoubleClick | 
        										ImGuiTreeNodeFlags_SpanAvailWidth;
        GameObject* go2remove = 0;


		const int size = obj->GetChildCount();
        for (int i = 0; i < size; ++i)
        {
            // Disable the default "open on single-click behavior" + set Selected flag according to our selection.
            ImGuiTreeNodeFlags node_flags = base_flags;
            GameObject* child = obj->GetChild( i );

            const bool isSelected = IsNodeSelected(child);
            if ( isSelected )
                node_flags |= ImGuiTreeNodeFlags_Selected;

            if( child->GetChildCount()==0 )
            {
            	node_flags |= ImGuiTreeNodeFlags_Leaf;// | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            }
            
            bool node_open = ImGui::TreeNodeEx((void*) child , node_flags, child->GetName() );
            if (ImGui::BeginPopupContextItem())
	        {
	        	is_GOContextMenuOpen = true;
	            if (ImGui::MenuItem("Add GameObject")) { AddGameObject( child ); }
	            if (ImGui::MenuItem("Remove GameObject")) { go2remove = child; }
	            RenameGameObject( child );
	            ImGui::EndPopup();
	        }

            if ( ImGui::IsItemClicked() ){
            	if( !ImGui::GetIO().KeyCtrl )
            		v_SelectedGameObjects.clear();

            	if( isSelected )
            		DeselectNode( child );
            	else
                	v_SelectedGameObjects.push_back( child );
            }
            if (node_open)
            {
                OnGUInode( child );
                ImGui::TreePop();
            }
            
        }

        if( go2remove!=0 )
        	RemoveGameObject( go2remove );
	}

	// Make the UI compact because there are so many fields
	static void PushStyleCompact()
	{
	    ImGuiStyle& style = ImGui::GetStyle();
	    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, (float)(int)(style.FramePadding.y * 0.60f)));
	    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, (float)(int)(style.ItemSpacing.y * 0.60f)));
	}

	static void PopStyleCompact()
	{
	    ImGui::PopStyleVar(2);
	}

	void HierarchyWindow::OnGUI()
	{
		static HierarchyWindow _this;
		static GameObject* go_root = &SYSTEMS::GetObject().SCENE.GetRootNode();
		auto window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;
		
		ImGui::Begin("Scene Hierarhy", NULL, window_flags); 


		is_GOContextMenuOpen = false;
        _this.OnGUInode( go_root );


        if (!is_GOContextMenuOpen && ImGui::BeginPopupContextWindow())
	    {
	        if (ImGui::MenuItem("Add GameObject")) { AddGameObject( go_root ); }
	        ImGui::EndPopup();
	    }

	    ImGui::End();

        if( _this.v_SelectedGameObjects.size() > 0)
        {
        	GameObjectViewWindow::SetSelectedGameObject( _this.v_SelectedGameObjects[0] );
        }
        else
        {
        	GameObjectViewWindow::SetSelectedGameObject( nullptr );
        }

	}
}

#endif