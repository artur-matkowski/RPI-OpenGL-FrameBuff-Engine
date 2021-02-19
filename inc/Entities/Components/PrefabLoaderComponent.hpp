#ifndef H_PrefabLoaderComponent
#define H_PrefabLoaderComponent
#include "ComponentBase.hpp"

namespace asapi
{
	class PrefabLoaderComponent: public ComponentBase<PrefabLoaderComponent>
	{
	protected:
		
	public:
		PrefabLoaderComponent(bfu::MemBlockBase* mBlock)
			:ComponentBase<PrefabLoaderComponent>(mBlock)
		{};
		~PrefabLoaderComponent(){};


		
	};
}

#endif