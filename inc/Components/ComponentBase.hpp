#ifndef _H_ComponentBase
#define _H_ComponentBase
#include "Serialization.hpp"


namespace asapgl
{
	class ComponentBase: public SERIALIZABLR_CLASS_BASE
	{
	public:	
		~ComponentBase(){};
		
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
	};
}

#endif