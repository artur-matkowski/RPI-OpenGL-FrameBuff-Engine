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
		,m_MaterialName(buffMat, 255, mBlock)
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
		SYSTEMS& systems = SYSTEMS::GetObject();
		
		systems.RESOURCES.requestResource( &m_material, m_MaterialName.c_str() );
		p_modelViewUniform = (UniformFloat4x4*) m_material->GetUniformPtr("modelViewMat");

		p_meshComponent = (MeshComponent*)m_owner->GET_COMPONENT(MeshComponent);

		if( m_material.GetRawPtr() != nullptr && p_meshComponent != nullptr)
		{
			//if you would ever try to update renderer component on the fly, you need to first unregister it from 
			//renderer system, to unbound mesh from material render queue.
			SYSTEMS::GetObject().RENDERER.RegisterRenderer( this );
		}
	}

	void RendererComponent::Render(glm::mat4* projectionMatrix, glm::mat4* viewMatrix)
	{
		if(p_modelViewUniform!=0)
			p_modelViewUniform->SetUniform( *projectionMatrix * *viewMatrix * *p_modelViewMat );
		else
			log::warning << "failing on matrix uniform update" << std::endl;


		m_material->BindMaterial();

		uint32_t* config = (uint32_t*)p_meshComponent->GetMeshResource()->GetRawHandle();

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
	
	#ifdef IS_EDITOR
	void RendererComponent::OnGUI()
	{

		{
			char buff1[255];
			strncpy(buff1, m_MaterialName.c_str(), 255 );

			if( ImGui::InputText("Material name",     buff1, 255, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue) )
			{
				log::debug << "updated material name " << buff1 << std::endl;

				m_MaterialName.clear();
				m_MaterialName.sprintf(buff1);

				OnIsDirty();
			}

		}
		if(m_material.GetRawPtr()!=nullptr)
			m_material->OnGUI();
	}
	#endif

}