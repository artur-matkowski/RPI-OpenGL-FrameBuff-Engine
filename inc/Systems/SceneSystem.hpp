#ifndef _H_SceneSystem
#define _H_SceneSystem
#include "GameObject.hpp"

namespace asapi
{
	class SceneSystem
	{
		GameObject  		*p_root = 0;

		bfu::JSONStream 	m_stream;
		char 				tmpbuff[2];

	public:
		SceneSystem( bfu::MemBlockBase* mBlock );

		inline GameObject& GetRootNode()
		{
			return *p_root;
		}

		void OnGUI();

		bfu::JSONStream& GetJSONStreamWorkBuffer();
	};
}

#endif