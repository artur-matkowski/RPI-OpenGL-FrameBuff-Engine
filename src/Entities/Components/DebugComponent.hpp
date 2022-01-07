#ifndef H_DebugComponent
#define H_DebugComponent
#include "ComponentBase.hpp"
#include "glm.hpp"
#include "Mesh_old.hpp"
#include "_ResourceProcessorsInclude.hpp"

namespace asapi
{
	using bfu::string;
	class DebugComponent: public ComponentBase<DebugComponent>
	{
	protected:
		//ResourceTXTSharedReference m_txtResourceRef;

		SERIALIZABLE_VAR_VEC(DebugComponent, string, m_meshName );
		SERIALIZABLE_OBJ(DebugComponent, UniqueID, m_test );
		SERIALIZABLE_GUI_OBJ(DebugComponent, ResourceTXTSharedReference, m_testResource );
		SERIALIZABLE_VAR_VEC(DebugComponent, bool, m_boolV );

		SERIALIZABLE_GUI_OBJ(DebugComponent, ResourceGLSLSharedReference, m_testShader );
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