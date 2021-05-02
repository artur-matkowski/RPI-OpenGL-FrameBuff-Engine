#include "Systems.hpp"
#include "RendererComponent.hpp"
#include "GameObject.hpp"
#include "ext.hpp"
#ifdef IS_EDITOR
#include "imgui.h"
#endif

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
		p_modelViewUniform = (Uniform<glm::mat4>*) m_material->GetUniformPtr("modelViewMat");

		p_meshComponent = (MeshComponent*)m_owner->GET_COMPONENT(MeshComponent);

		if( m_material.GetRawPtr() != nullptr && p_meshComponent != nullptr)
		{
			SYSTEMS::GetObject().RENDERER.RegisterRenderer( this );
		}
	}

	void RendererComponent::Render(glm::mat4* projectionMatrix)
	{
		if(p_modelViewUniform!=0)
			p_modelViewUniform->SetUniform( *projectionMatrix * *p_modelViewMat );
		else
			log::warning << "failing on matrix uniform update" << std::endl;


		m_material->BindMaterial();

		p_meshComponent->GetMeshResource()->Render();
		//m_mesh->Render();
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