#include "PointerColider.hpp"
#include "Systems.hpp"
#include "GameObject.hpp"
#include "ext.hpp"
#ifdef IS_EDITOR
#include "imgui.h"
#endif

namespace asapi
{
	PointerColider::PointerColider(bfu::MemBlockBase* mBlock)
		:ComponentBase<PointerColider>(mBlock)
	{
	};

	void PointerColider::OnAttach()
	{
		p_modelViewMat = ((Transform3D*)m_owner->GET_COMPONENT(Transform3D))->GetModelMatrix();

		#ifdef IS_EDITOR
		OnIsDirty();
		#endif
	}
	void PointerColider::OnDetach()
	{
		SYSTEMS::GetObject().RENDERER.UnRegisterPointerColider( this );		
	}

	void PointerColider::OnIsDirty()
	{
		p_meshComponent = (MeshComponent*)m_owner->GET_COMPONENT(MeshComponent);

		if( p_meshComponent != nullptr
			&& p_meshComponent->GetMeshHandle() != nullptr )
		{
			//if you would ever try to update renderer component on the fly, you need to first unregister it from 
			//renderer system, to unbound mesh from material render queue.
			SYSTEMS::GetObject().RENDERER.UnRegisterPointerColider( this );
			SYSTEMS::GetObject().RENDERER.RegisterPointerColider( this );
		}
	}

	void PointerColider::Render(glm::mat4* projectionMatrix, glm::mat4* viewMatrix)
	{
	}

	void PointerColider::PreSerializationCallback()
	{
	}
	void PointerColider::PostDeserializationCallback()
	{
		OnIsDirty();
	}
	
	#ifdef IS_EDITOR
	void PointerColider::OnGUI()
	{
		if( p_meshComponent != nullptr
			&& p_meshComponent->GetMeshHandle() != nullptr )
		{
			ImGui::TextColored(
						ImVec4(0.0f, 1.0f, 0.0f, 1.0f)
						, "PointerColider is Valid");
		}
		else
		{
			ImGui::TextColored(
						ImVec4(1.0f, 0.0f, 0.0f, 1.0f)
						, "PointerColider is Invalid");			
		}
	}
	#endif
}