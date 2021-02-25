#ifndef H_PrefabLoaderComponent
#define H_PrefabLoaderComponent
#include "ComponentBase.hpp"

namespace asapi
{
	class PrefabLoaderComponent: public ComponentBase<PrefabLoaderComponent>
	{
	protected:
		Serializable<bfu::string> m_prefabName;
	public:
		PrefabLoaderComponent(bfu::MemBlockBase* mBlock)
			:ComponentBase<PrefabLoaderComponent>(mBlock)
			,m_prefabName("m_prefabName", this, mBlock)
		{};
		PrefabLoaderComponent(const PrefabLoaderComponent&) = delete;
		~PrefabLoaderComponent(){};


		//virtual void OnGUI() override;
	};
}

#endif