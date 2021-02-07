#ifndef _H_SceneSystem
#define _H_SceneSystem
#include "GameObjectLoader.hpp"

namespace asapgl
{
	class SceneSystem
	{
		GameObjectLoader  	*p_root = 0;

		#ifdef IS_EDITOR
		bfu::JSONStream 	m_stream;
		char 				tmpbuff[2];
		#endif

	public:
		SceneSystem( bfu::MemBlockBase* mBlock );

		inline GameObjectLoader& GetRootNode()
		{
			return *p_root;
		}

		void OnGUI();

		#ifdef IS_EDITOR
		bfu::JSONStream& GetJSONStreamWorkBuffer();
		#endif
	};
}

#endif