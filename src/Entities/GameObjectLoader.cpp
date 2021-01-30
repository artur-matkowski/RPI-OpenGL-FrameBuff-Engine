#include "GameObjectLoader.hpp"

namespace asapgl
{
	GameObjectLoader::GameObjectLoader( bfu::MemBlockBase* mBlock )
		:GameObject(mBlock)
		,m_prefabMemFile("m_prefabMemFile", this, mBlock)
	{

	}
	GameObjectLoader::~GameObjectLoader()
	{

	}
}