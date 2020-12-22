#ifndef _H_ComponentBase
#define _H_ComponentBase

namespace asapgl
{
	class ComponentBase
	{
	public:
		ComponentBase(){};
		~ComponentBase(){};
		
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
	};
}

#endif