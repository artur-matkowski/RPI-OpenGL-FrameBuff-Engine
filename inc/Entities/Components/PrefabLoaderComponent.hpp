#ifndef H_PrefabLoaderComponent
#define H_PrefabLoaderComponent
#include "ComponentBase.hpp"

namespace asapi
{
	class PrefabLoaderComponent: public ComponentBase<PrefabLoaderComponent>
	{
	protected:
		bfu::SerializableVar<bfu::string> m_prefabName;
	public:
		PrefabLoaderComponent(bfu::MemBlockBase* mBlock)
			:ComponentBase<PrefabLoaderComponent>(mBlock)
			,m_prefabName("prefabName", this, mBlock)
		{};
		~PrefabLoaderComponent(){};


		
	};
}

#endif