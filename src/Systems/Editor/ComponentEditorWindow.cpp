#include "ComponentEditorWindow.hpp"
#include "GameObject.hpp"
#include "imgui.h"
#include <cstdio>
#include <iostream>
#ifdef IS_EDITOR

namespace asapi
{

	void print(Node<size_t>& f, int depth = 0)
	{
		std::cout << "\n";

		for(int i=0; i<depth; ++i) std::cout << "  ";

		std::cout << f.name();

		for(int i=0; i<f.size(); ++i)
		{
			print(*f[i], depth+1);
		}
	}

	ComponentEditorWindow::ComponentEditorWindow()
	{
		//log::error << "ComponentEditorWindow::ComponentEditorWindow()" << std::endl;

		TypeInfo* types = TypeInfo::GetTypeInfo();
		int typesC = TypeInfo::GetTypeInfoSize();
		TypeInfo* end = types + typesC;

		for(TypeInfo* it = types; it!=end; ++it)
		{
			m_rootNode.FeedEntry( it->name, ":", it->id);
		}

		print(m_rootNode);
	};
	ComponentEditorWindow::~ComponentEditorWindow(){};

	GameObject* _selected = 0;

	void ComponentEditorWindow::SetSelectedGameObject( GameObject* ptr )
	{
		_selected = ptr;
	}

	void OnGUI(Node<size_t>* obj)
	{
		if( obj->size() > 0)
		{
			if (ImGui::BeginMenu( obj->name() ))
            {
            	for(int i=0; i<obj->size(); ++i)
            	{
            		OnGUI( (*obj)[i] );
            	}
                ImGui::EndMenu();
            }
		}
		else
		{
			if( ImGui::MenuItem( obj->name() ) )
			{
				_selected->AddComponent( obj->m_infoBuffer );
			}
		}
	}

	ComponentInterface* p_forRemoval = nullptr;
	void ComponentEditorWindow::OnGUI(ComponentInterface* obj)
	{
		ImGui::LabelText( "Component", obj->TypeName() ); 
		ImGui::SameLine();
		ImGui::PushItemWidth(-(ImGui::GetWindowContentRegionWidth() - ImGui::CalcItemWidth()));
		ImGui::PushID( obj );
		if( ImGui::Button("Remove Component") )
		{
			p_forRemoval = obj;
		}
		ImGui::PopID();
		ImGui::PopItemWidth();

		obj->OnGUI();
	}

	void ComponentEditorWindow::OnGUI(GameObject* obj)
	{
		ImGui::Spacing();
		ImGui::Text( obj->GetName() );

		for(int i=0; i<obj->GetComponentsCount(); ++i)
		{
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			//obj->GetComponent(i)->p_ComponentInterface->OnGUI_NameAndVirtual();
			OnGUI( obj->GetComponent(i)->p_ComponentInterface );
		}
	}

	void ComponentEditorWindow::OnGUI()
	{
		static ComponentEditorWindow _this;
		auto window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;

		ImGui::Begin("GameObject View", NULL, window_flags); 

		if( _selected!=0 )
		{
			OnGUI(_selected);

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
				
			if( ImGui::Button("Add Component") )
				ImGui::OpenPopup("Add_Component_popup");
		    if (ImGui::BeginPopup("Add_Component_popup"))
			{
	            asapi::OnGUI( &_this.m_rootNode );

	            ImGui::EndPopup();
			}
		}

	    ImGui::End();

	    if( p_forRemoval!=0 ){
			p_forRemoval->GetOwner()->RemoveComponent( p_forRemoval );
			p_forRemoval = 0;
		}
	}
}

#endif