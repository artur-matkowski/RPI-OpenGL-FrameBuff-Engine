#include "GameObjectViewWindow.hpp"
#include "GameObject.hpp"
#include "imgui.h"
#include <cstdio>
#include <iostream>

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

	GameObjectViewWindow::GameObjectViewWindow()
	{
		//log::error << "GameObjectViewWindow::GameObjectViewWindow()" << std::endl;

		TypeInfo* types = TypeInfo::GetTypeInfo();
		int typesC = TypeInfo::GetTypeInfoSize();
		TypeInfo* end = types + typesC;

		for(TypeInfo* it = types; it!=end; ++it)
		{
			m_rootNode.FeedEntry( it->name, ":", it->id);
		}

		print(m_rootNode);
	};
	GameObjectViewWindow::~GameObjectViewWindow(){};

	GameObject* _selected = 0;

	void GameObjectViewWindow::SetSelectedGameObject( GameObject* ptr )
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

	void GameObjectViewWindow::OnGUI()
	{
		static GameObjectViewWindow _this;
		auto window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;

		ImGui::Begin("GameObject View", NULL, window_flags); 

		if( _selected!=0 )
		{
			_selected->OnGUI();

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


	}
}

