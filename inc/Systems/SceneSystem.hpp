#ifndef _H_SceneSystem
#define _H_SceneSystem
#include "GameObjectLoader.hpp"

namespace asapgl
{
	class SceneSystem
	{
		GameObjectLoader  m_root;
	public:
		SceneSystem( bfu::MemBlockBase* mBlock );

		inline GameObjectLoader& GetRootNode()
		{
			return m_root;
		}

		void OnGUI();


	};
}

#endif