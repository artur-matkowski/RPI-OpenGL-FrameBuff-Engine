#ifndef H_DebugComponent
#define H_DebugComponent
#include "ComponentBase.hpp"
#include "glm.hpp"
#include "Mesh_old.hpp"
#include "_ResourceProcessorsInclude.hpp"
#include "SerializableObject.hpp"

namespace asapi
{
	class TestSerialization: public SerializableObject<TestSerialization>
	{
	public:
		SERIALIZABLE_VAR(TestSerialization, float, b);

		/*virtual void OnGUI() override
		{
			printf("b is equal to %f", b);
		}*/
	};


	using bfu::string;
	class DebugComponent: public ComponentBase<DebugComponent>
	{
	protected:
		//ResourceTXTSharedReference m_txtResourceRef;

		SERIALIZABLE_VAR_VEC(DebugComponent, string, m_meshName );
		SERIALIZABLE_OBJ(DebugComponent, UniqueID, m_test );
		//SERIALIZABLE_OBJ(DebugComponent, ResourceTXTSharedReference, m_testResource );
		SERIALIZABLE_VAR_VEC(DebugComponent, bool, m_boolV );

		SERIALIZABLE_OBJ(DebugComponent, TestSerialization, m_testobj );

		//SERIALIZABLE_OBJ(DebugComponent, ResourceGLSLSharedReference, m_testShader );
	public:
		DebugComponent(bfu::MemBlockBase* mBlock);
		DebugComponent(const DebugComponent&) = delete;
		~DebugComponent(){};


		//virtual void OnAttach() override;
		virtual void OnIsDirty() override;
		
		/*
		#ifdef IS_EDITOR
		virtual void OnGUI() override;
		#endif
		*/
	};
}

#endif