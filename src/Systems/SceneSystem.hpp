#ifndef _H_SceneSystem
#define _H_SceneSystem
#include "GameObject.hpp"

namespace asapi
{
	class SceneSystem
	{
		GameObject  		*p_root = 0;

		bfu::JSONSerializer m_jsonSerializer;
		char 				tmpbuff[2];

	public:
		SceneSystem();

		inline GameObject& GetRootNode()
		{
			return *p_root;
		}

		void Init( bfu::MemBlockBase* mBlock, const int argc, const char** argv );

		bfu::JSONSerializer& GetJSONSerializer();

		static bool File2JSON(bfu::JSONSerializer&, const char* path);
		static bool JSON2File(bfu::JSONSerializer&, const char* path);
		static bool File2JSON(bfu::SerializableClassInterface*, const char* path);
		static bool JSON2File(bfu::SerializableClassInterface*, const char* path);

		bool OpenProject();
		bool LoadRootMMP();
	};
}

#endif