#include "Systems.hpp"
#include "RendererComponent.hpp"

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
}