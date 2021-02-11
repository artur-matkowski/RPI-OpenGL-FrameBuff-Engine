#include "ComponentBase.hpp"
#include "Systems.hpp"
#include "imgui.h"

namespace asapgl
{

	void ComponentInterface::OnGUInamed(const char* ComponentName)
	{
		//TODO render serializable fields in imgui
		
		ImGui::Text("Debug Component renderer for component %s", ComponentName); 
	}
}