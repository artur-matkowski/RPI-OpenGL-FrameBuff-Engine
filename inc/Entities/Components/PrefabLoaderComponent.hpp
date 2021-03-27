#ifndef H_PrefabLoaderComponent
#define H_PrefabLoaderComponent
#include "ComponentBase.hpp"
#include "PrefabMemBlock.hpp"

namespace asapi
{
	class PrefabLoaderComponent: public ComponentBase<PrefabLoaderComponent>
	{
	protected:	
		SERIALIZABLE_OBJ( PrefabLoaderComponent, UniqueID, m_prefabID );
		PrefabMemBlock* 				m_prefabMemBlock = nullptr;
	public:
		PrefabLoaderComponent(bfu::MemBlockBase* mBlock)
			:ComponentBase<PrefabLoaderComponent>(mBlock)
		{};
		PrefabLoaderComponent(const PrefabLoaderComponent&) = delete;
		~PrefabLoaderComponent(){};


		#ifdef IS_EDITOR
		virtual void OnGUI() override;
		#endif

		virtual void OnAttach() override;
		PrefabMemBlock* RequestPrefabMemBlock();

		void Save_JSON();
		bool Load_JSON();
		void Save_MMP();
		void Load_MMP();
		void UnLoad();



		void SetPrefabID(uint64_t id);
		uint64_t GetPrefabID();
	};
}

#endif