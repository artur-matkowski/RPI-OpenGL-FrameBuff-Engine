#ifndef _H_ComponentBase
#define _H_ComponentBase
#include "EntityBase.hpp"


namespace asapgl
{
	class ComponentBase: public EntityBase
	{
	public:	
		ComponentBase(bfu::MemBlockBase* mBlock)
			:EntityBase(mBlock)
		{};
		~ComponentBase(){};
		
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
	};
}

#endif