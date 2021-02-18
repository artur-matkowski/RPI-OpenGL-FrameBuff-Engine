#include "SceneSystem.hpp"
#include "ImguiXlib.hpp"
#include "Systems.hpp"

namespace asapi
{

	SceneSystem::SceneSystem( bfu::MemBlockBase* mBlock )
		:m_stream(tmpbuff, 2/*, mBlock*/) // using std allocator to dont waste large chunks of memory on reallocations
	{
		m_stream.resize(1024*1024*10); // reserve 10 Mb buff for reading json

		p_root = (GameObject*) mBlock->allocate(1, sizeof(GameObject), alignof(GameObject));
		p_root->Init(mBlock);
		p_root->SetName("EntryPoint");

		GameObject *pgo, *pgo2 ;

		pgo2 = pgo = SYSTEMS::ALLOCATE<GameObject>(1);
		pgo->Init(mBlock);
		pgo->OnAttach(p_root);
		pgo->SetName("GameObject 1");

		pgo = SYSTEMS::ALLOCATE<GameObject>(1);
		pgo->Init(mBlock);
		pgo->OnAttach(pgo2);
		pgo->SetName("GameObject 2");

		pgo = SYSTEMS::ALLOCATE<GameObject>(1);
		pgo->Init(mBlock);
		pgo->OnAttach(p_root);
		pgo->SetName("GameObject 3");

	}

	void SceneSystem::OnGUI()
	{
		
	}


	#ifdef IS_EDITOR
	bfu::JSONStream& SceneSystem::GetJSONStreamWorkBuffer()
	{
		return m_stream;
	}
	#endif
}