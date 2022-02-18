#include "SerializableObject.hpp"
#include "UniqueID.hpp"

namespace asapi
{
	class MaterialReference: public SerializableObject<MaterialReference>
	{
	protected:
		SERIALIZABLE_OBJ(MaterialReference, UniqueID, m_materialTypeID);

		MaterialReference(){};
	public:
		static MaterialReference LoadMaterial(const UniqueID& uuid);
		MaterialReference(const MaterialReference& cp){};
	};
}