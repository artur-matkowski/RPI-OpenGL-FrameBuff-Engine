#ifndef _H_RendererComponent
#define _H_RendererComponent
#include "ComponentBase.hpp"

namespace asapgl
{
	class RendererComponent: public ComponentBase
	{

	public:
		RendererComponent(){};
		~RendererComponent(){};


		virtual void OnAttach(){};
		virtual void OnDetach(){};


		inline void Render();
		
	};
}

#endif