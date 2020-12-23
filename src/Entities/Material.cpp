#include "Material.hpp"
#include "Systems.hpp"

namespace asapgl
{
	Material::Material(const char* materialName)
		:bfu::SerializableClassBase()
		,m_usedShader("m_usedShader", this)
		,m_usedTextures("m_usedTextures", this)
	{
		SYSTEMS& systems = SYSTEMS::GetObject();

		systems.RESOURCES.requestResource( &m_shader, "---" );


		ResourcePtr< Texture > tmp;
		systems.RESOURCES.requestResource( &tmp, "debug.png" );
		m_textures.push_back(tmp);

	}
}