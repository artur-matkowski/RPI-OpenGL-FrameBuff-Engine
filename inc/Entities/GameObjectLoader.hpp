#ifndef _H_GameObjectLoader
#define _H_GameObjectLoader
#include "GameObject.hpp"
#include <bitforge/utils/bfu.hpp>

namespace asapgl
{
	class GameObjectLoader: public GameObject
	{
		bfu::SerializableVar<bfu::string>  m_prefabMemFile;
	public:
		GameObjectLoader( bfu::MemBlockBase* mBlock );
		~GameObjectLoader();


		bool LoadContent();
		
	};
}

#endif