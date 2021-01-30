#ifndef _H_PrefabLoader
#define _H_PrefabLoader
#include "object.hpp"
#include <bitforge/utils/bfu.hpp>

namespace asapgl
{
	class PrefabLoader: public object
	{
		bfu::MemBlockBase* m_mBlock = 0;
	public:
		PrefabLoader( bfu::MemBlockBase* mBlock );
		~PrefabLoader();
		
	};
}

#endif