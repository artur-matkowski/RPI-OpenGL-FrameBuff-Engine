#include "Systems.hpp"
#include "RendererComponent.hpp"
#include "GameObject.hpp"
#include "ext.hpp"
#ifdef IS_EDITOR
#include "imgui.h"
#endif
#include <GLES2/gl2.h>

namespace asapi
{
	RendererComponent::RendererComponent(bfu::MemBlockBase* mBlock)
		:ComponentBase<RendererComponent>(mBlock)
	{
	};

	void RendererComponent::OnAttach()
	{
		p_modelViewMat = ((Transform3D*)m_owner->GET_COMPONENT(Transform3D))->GetModelMatrix();

	}
	void RendererComponent::OnDetach()
	{
		SYSTEMS::GetObject().RENDERER.UnRegisterRenderer( this );
	}
	void RendererComponent::OnIsDirty()
	{
		p_meshComponent = (MeshComponent*)m_owner->GET_COMPONENT(MeshComponent);

		if( m_incomingMaterialImpl.IsValid() 
			&& p_meshComponent != nullptr
			&& p_meshComponent->GetMeshHandle() != nullptr )
		{
			//if you would ever try to update renderer component on the fly, you need to first unregister it from 
			//renderer system, to unbound mesh from material render queue.
			SYSTEMS::GetObject().RENDERER.UnRegisterRenderer( this );
			SYSTEMS::GetObject().RENDERER.RegisterRenderer( this );
		}
	}

	void RendererComponent::Render(glm::mat4* projectionMatrix, glm::mat4* viewMatrix)
	{
		//#ifdef IS_EDITOR
		if( !m_incomingMaterialImpl.GetMaterialInstance()->IsValid() )
		{
			return;
		}
		//#endif

		m_incomingMaterialImpl.GetMaterialInstance()->GetModelViewMatrix()->SetUniform( *projectionMatrix * *viewMatrix * *p_modelViewMat );
		m_incomingMaterialImpl.GetMaterialInstance()->BindMaterial();

		uint32_t* config = (uint32_t*)p_meshComponent->GetMeshHandle();

		// //Render
		glBindBuffer(GL_ARRAY_BUFFER, config[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, config[1]);

		const uint32_t attributes = config[2];
		for(uint32_t i = 0; i<attributes; ++i)
		{
			const uint32_t index = i*5+3;
			glEnableVertexAttribArray(config[index+0]);
			glVertexAttribPointer(config[index+1]
								, config[index+2]
								, GL_FLOAT
								, GL_FALSE
								, sizeof(GL_FLOAT)*(config[index+3])
								, (void*) (sizeof(GL_FLOAT)*(config[index+4])) );
		}


		const uint32_t size = config[3+attributes*5];
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, NULL);

		// glDisableVertexAttribArray(0);
		// glDisableVertexAttribArray(2);
	}
	
	void RendererComponent::PreSerializationCallback()
	{
	}
	void RendererComponent::PostDeserializationCallback()
	{
		OnIsDirty();
	}

	#ifdef IS_EDITOR
	void RendererComponent::OnGUI()
	{
        m_incomingMaterialImpl.OnGUI_caller();
	}
	#endif

}