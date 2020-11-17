#ifndef _H_SYSTEMS
#define _H_SYSTEMS
#include "Singleton.hpp"
#include "object.hpp"
#include <bitforge/utils/bfu.hpp>

namespace asapgl
{
	class SYSTEMS: public object, public Singleton<SYSTEMS>
	{
	public:
		bfu::EventSystem EVENTS;
	
	};

}

#endif