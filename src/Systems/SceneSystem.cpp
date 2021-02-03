#include "SceneSystem.hpp"
#include "ImguiXlib.hpp"
#include "Systems.hpp"

namespace asapgl
{

	SceneSystem::SceneSystem( bfu::MemBlockBase* mBlock )
		:m_root(mBlock)
	{
		m_root.SetName("root node");
		m_root.SetPrefabMemFileName("EntryPoint.mmp");

		GameObject *pgo, *pgo2 ;

		pgo2 = pgo = SYSTEMS::ALLOCATE<GameObject>(1);
		pgo->Init(mBlock);
		pgo->OnLoad(&m_root);
		pgo->SetName("GameObject 1");

		pgo = SYSTEMS::ALLOCATE<GameObject>(1);
		pgo->Init(mBlock);
		pgo->OnLoad(pgo2);
		pgo->SetName("GameObject 2");

		pgo = SYSTEMS::ALLOCATE<GameObject>(1);
		pgo->Init(mBlock);
		pgo->OnLoad(&m_root);
		pgo->SetName("GameObject 3");

	}

	void SceneSystem::OnGUI()
	{
		
	}
}