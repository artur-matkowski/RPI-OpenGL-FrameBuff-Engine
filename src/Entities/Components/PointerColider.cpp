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
	}
	void PointerColider::OnDetach()
	{
		
	}

	void PointerColider::OnIsDirty()
	{
		SYSTEMS& systems = SYSTEMS::GetObject();
		
		p_meshComponent = (MeshComponent*)m_owner->GET_COMPONENT(MeshComponent);
	}

	void PointerColider::Render(glm::mat4* projectionMatrix)
	{
	}
	
	#ifdef IS_EDITOR
	void PointerColider::OnGUI()
	{

	}
	#endif
}