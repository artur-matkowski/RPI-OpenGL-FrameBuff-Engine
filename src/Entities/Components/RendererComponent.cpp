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
		const float *pSource = (const float*)glm::value_ptr( m_owner->GetTransform3D()->GetModelMatrix() );
		//glLoadMatrixf( pSource );
		//glUniformMatrix4fv(0, 16, false, pSource);

		m_material->BindMaterial();
		m_mesh->Render();
	}

}