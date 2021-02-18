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


		void Init( bfu::MemBlockBase* mBlock )
		{
			new (this) PrefabLoaderComponent(mBlock);
		};

		virtual void OnAttach();
		virtual void OnDetach();
	};
}

#endif