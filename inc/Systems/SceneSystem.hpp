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

		void Init( bfu::MemBlockBase* mBlock );

		void OnGUI();

		bfu::JSONStream& GetJSONStreamWorkBuffer();

		static bool File2JSON(bfu::JSONStream&, const char* path);
		static bool JSON2File(bfu::JSONStream&, const char* path);
	};
}

#endif