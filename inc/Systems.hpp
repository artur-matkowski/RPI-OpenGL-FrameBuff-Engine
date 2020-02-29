#ifndef _H_SYSTEMS
#define _H_SYSTEMS
#include "Singleton.hpp"
#include "object.hpp"

namespace asapgl
{
	class SYSTEMS: public object, public Singleton<SYSTEMS>
	{
	public:
		static const char* test;
	
	};
}

#endif