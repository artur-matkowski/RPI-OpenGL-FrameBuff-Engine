#include "ComponentBase.hpp"
#include "Systems.hpp"
#include "imgui.h"

namespace asapgl
{
	std::unordered_map<size_t
						,ComponentInterface::InitFuncPtr
						,std::hash<size_t>
						,std::equal_to<size_t>
						,bfu::custom_allocator< std::pair<const size_t, ComponentInterface::InitFuncPtr> >
						> 
						ComponentInterface::s_componentAllocatorMap( &SYSTEMS::GetObject().MEMORY.GetSystemsAllocator() );


	void ComponentInterface::OnGUI()
	{
		//TODO render serializable fields in imgui
		
		ImGui::Text("Debug Component renderer"); 
	}
}