#ifndef _H_SceneSystem
#define _H_SceneSystem
#include "GameObjectLoader.hpp"

namespace asapgl
{
	class SceneSystem
	{
		GameObjectLoader  	m_root;

		#ifdef IS_EDITOR
		bfu::JSONStream 	m_stream;
		char 				tmpbuff[2];
		#endif

	public:
		SceneSystem( bfu::MemBlockBase* mBlock );

		inline GameObjectLoader& GetRootNode()
		{
			return m_root;
		}

		void OnGUI();

		#ifdef IS_EDITOR
		bfu::JSONStream& GetJSONStreamWorkBuffer();
		#endif
	};
}

#endif