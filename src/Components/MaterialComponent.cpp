#include "Systems.hpp"
#include "MaterialComponent.hpp"

namespace asapgl
{
	MaterialComponent::MaterialComponent()
		:ComponentBase()
		,m_usedShader("m_usedShader",this)
		,m_usedTextures("m_usedTextures",this)
	{
		SYSTEMS& systems = SYSTEMS::GetObject();
		ResourcePtr< Texture > texture;
		
		systems.RESOURCES.requestResource( &m_shader, "---" );
		
		systems.RESOURCES.requestResource( &texture, "debug.png" );
		m_textures.push_back( texture );
	};
}