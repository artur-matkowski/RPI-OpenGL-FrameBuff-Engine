#include "MaterialsSystem.hpp"

#ifdef IS_EDITOR
#include "imgui.h"
#endif


namespace asapi
{
	std::string MaterialsSystem::s_projectPath;


	void MaterialsSystem::SetProjectPath( const std::string& path )
	{
		s_projectPath = path;
	}

	void MaterialsSystem::RefreshResources()
	{

	}

	#ifdef IS_EDITOR
	void MaterialsSystem::OnGUI()
	{
		if( ImGui::Button("Create new Material Type") )
		{

		}
	}
	#endif
}