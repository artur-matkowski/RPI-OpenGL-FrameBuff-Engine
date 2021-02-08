#include "GameObjectViewWindow.hpp"
#include "GameObject.hpp"
#include "imgui.h"

namespace asapgl
{
	GameObjectViewWindow::GameObjectViewWindow(){};
	GameObjectViewWindow::~GameObjectViewWindow(){};

	GameObject* _selected = 0;

	void GameObjectViewWindow::SetSelectedGameObject( GameObject* ptr )
	{
		_selected = ptr;
	}

	void GameObjectViewWindow::OnGUI()
	{
		static GameObjectViewWindow _this;
		auto window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;

		ImGui::Begin("GameObject View", NULL, window_flags); 

		if( _selected!=0 )
		{
			_selected->OnGUI();
		}

	    ImGui::End();


	}
}

