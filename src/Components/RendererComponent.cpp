#include "Systems.hpp"
#include "RendererComponent.hpp"

namespace asapgl
{
	RendererComponent::RendererComponent()
		:ComponentBase()
		,SERIALIZABLE_VAR_CONSTRUCTOR(m_usedMaterial,this)
		,SERIALIZABLE_VAR_CONSTRUCTOR(m_usedMesh,this)
	{
		SYSTEMS& systems = SYSTEMS::GetObject();
		
		systems.RESOURCES.requestResource( &m_material, "---" );
		
		systems.RESOURCES.requestResource( &m_mesh, "---" );
	};
}