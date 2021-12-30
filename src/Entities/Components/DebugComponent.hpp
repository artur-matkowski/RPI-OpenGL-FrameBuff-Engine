#ifndef H_DebugComponent
#define H_DebugComponent
#include "ComponentBase.hpp"
#include "glm.hpp"
#include "Mesh_old.hpp"

namespace asapi
{
	using bfu::string;
	class DebugComponent: public ComponentBase<DebugComponent>
	{
	protected:
		//ResourceTXTSharedReference m_txtResourceRef;

		SERIALIZABLE_VAR_VEC(DebugComponent, string, m_meshName );
		SERIALIZABLE_OBJ(DebugComponent, UniqueID, m_test );
		SERIALIZABLE_VAR_VEC(DebugComponent, bool, m_boolV );

	public:
		DebugComponent(bfu::MemBlockBase* mBlock);
		DebugComponent(const DebugComponent&) = delete;
		~DebugComponent(){};


		//virtual void OnAttach() override;
		virtual void OnIsDirty() override;
		#ifdef IS_EDITOR
		//virtual void OnGUI() override;
		#endif

	};
}

#endif