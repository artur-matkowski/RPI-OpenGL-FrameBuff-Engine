#include "Systems.hpp"
#include "RendererComponent.hpp"
#include "GameObject.hpp"
#include "glm.hpp"
#include "ext.hpp"
#include "imgui.h"

namespace asapi
{
	RendererComponent::RendererComponent(bfu::MemBlockBase* mBlock)
		:ComponentBase<RendererComponent>(mBlock)
		//,m_usedMaterial("m_usedMaterial",this, mBlock)
		//,m_usedMesh("m_usedMesh",this, mBlock)
	{
		SYSTEMS& systems = SYSTEMS::GetObject();
		
		systems.RESOURCES.requestResource( &m_material, "---" );
		
		systems.RESOURCES.requestResource( &m_mesh, "---" );

		p_modelViewMat = (Uniform<glm::mat4>*) m_material->GetUniformPtr("modelViewMat");
	};

	void RendererComponent::OnAttach()
	{
		SYSTEMS::GetObject().RENDERER.RegisterRenderer( this );
	}
	void RendererComponent::OnDetach()
	{
		SYSTEMS::GetObject().RENDERER.UnRegisterRenderer( this );
	}

	void RendererComponent::Render()
	{
		ptr = (void*)m_owner->GetTransform3D();
		glm::mat4 *tmp = &m_owner->GetTransform3D()->GetMVMatrix();


		if(p_modelViewMat!=0)
			p_modelViewMat->SetUniform( *tmp );
		else
			log::warning << "failing on matrix uniform update" << std::endl;

		m_material->BindMaterial();
		m_mesh->Render();
	}
	
	void RendererComponent::OnGUI()
	{
		ImGui::Text(" %s %lld", m_owner->GetName(), (size_t)ptr );
	}

}