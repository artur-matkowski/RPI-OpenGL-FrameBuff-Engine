#ifndef _H_ComponentBase
#define _H_ComponentBase
#include <bitforge/utils/bfu.hpp>	


namespace asapgl
{
	class ComponentBase: public bfu::SerializableClassBase
	{
	public:
		ComponentBase()
			:bfu::SerializableClassBase()
		{};
		ComponentBase(const ComponentBase& copy)
			:bfu::SerializableClassBase()
		{};		
		~ComponentBase(){};
		
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
	};
}

#endif