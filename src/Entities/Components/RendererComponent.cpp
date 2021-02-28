#include "Systems.hpp"
#include "RendererComponent.hpp"
#include "GameObject.hpp"
#include "glm.hpp"
#include "ext.hpp"
#include <GLES3/gl3.h> 


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
		p_modelViewMat->SetUniform( m_owner->GetTransform3D()->GetMVMatrix() );

		m_material->BindMaterial();
		m_mesh->Render();
	}

}