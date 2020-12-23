#include "Systems.hpp"
#include "RendererComponent.hpp"

namespace asapgl
{
	RendererComponent::RendererComponent()
		:ComponentBase()
		,m_usedMaterial("m_usedMaterial",this)
		,m_usedMesh("m_usedMesh",this)
	{
		SYSTEMS& systems = SYSTEMS::GetObject();
		
		systems.RESOURCES.requestResource( &m_material, "---" );
		
		systems.RESOURCES.requestResource( &m_mesh, "---" );
	};
}