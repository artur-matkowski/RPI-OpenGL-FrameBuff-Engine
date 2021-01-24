#ifndef _H_ComponentBase
#define _H_ComponentBase
#include "Serialization.hpp"


namespace asapgl
{
	class ComponentBase: public object//, public bfu::SerializableClassBase
	{
	public:	
		ComponentBase(bfu::MemBlockBase* mBlock)
			//:bfu::SerializableClassBase(mBlock)
		{};
		~ComponentBase(){};
		
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
	};
}

#endif