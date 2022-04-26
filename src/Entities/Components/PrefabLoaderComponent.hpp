#ifndef H_PrefabLoaderComponent
#define H_PrefabLoaderComponent
#include "ComponentBase.hpp"

namespace asapi
{
	class PrefabLoaderComponent: public ComponentBase<PrefabLoaderComponent>
	{
	public:
		static std::string s_projectPath; //TODO fix that crap
	protected:	
		SERIALIZABLE_OBJ( PrefabLoaderComponent, UniqueID, m_prefabID );
		bfu::MemBlockBase* 				m_prefabMemBlock = nullptr;
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
		bfu::MemBlockBase* RequestPrefabMemBlock();

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