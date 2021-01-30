#include "SceneSystem.hpp"
#include "ImguiXlib.hpp"


namespace asapgl
{

	SceneSystem::SceneSystem( bfu::MemBlockBase* mBlock )
		:m_root(mBlock)
	{

	}

	void SceneSystem::OnGUI()
	{
		ImGui::Begin("Scene Hierarhy"); 

	    ImGui::End();
	}
}