#ifndef H_PrefabLoaderComponent
#define H_PrefabLoaderComponent
#include "ComponentBase.hpp"

namespace asapgl
{
	class PrefabLoaderComponent: public ComponentBase
	{
	protected:
	public:
		virtual void OnAttach();
		virtual void OnDetach();
	};
}

#endif