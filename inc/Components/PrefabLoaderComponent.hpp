#ifndef H_PrefabLoaderComponent
#define H_PrefabLoaderComponent
#include "ComponentBase.hpp"

namespace asapgl
{
	class PrefabLoaderComponent: public ComponentBase<PrefabLoaderComponent>
	{
	protected:
	public:
		PrefabLoaderComponent(bfu::MemBlockBase* mBlock)
			:ComponentBase<PrefabLoaderComponent>(mBlock)
		{};
		~PrefabLoaderComponent();

		virtual void OnAttach();
		virtual void OnDetach();
	};
}

#endif