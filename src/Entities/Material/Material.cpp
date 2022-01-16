#include "Material.hpp"
#include "PersistanceSystem.hpp"
#include "ImGUI_Serializer.hpp"

namespace asapi
{
	void Material::OnGUI()
	{
		ImGUI_Serializer guiSerializer;

		guiSerializer.Serialize( this );
	}
}