#ifndef H_MaterialReference
#define H_MaterialReference

#include "SerializableObject.hpp"
#include "UniqueID.hpp"
#include "MaterialType2.hpp"

namespace asapi
{
	class MaterialReference: public SerializableObject<MaterialReference>
	{
	protected:
		SERIALIZABLE_OBJ(MaterialReference, UniqueID, m_materialTypeID);

		MaterialType2* 		m_materialType = nullptr;
		uint16_t* 			m_referenceCounter = nullptr;


		MaterialReference();
	public:
		static MaterialReference LoadMaterial(const std::string projectPath, const UniqueID& uuid);
		MaterialReference(const MaterialReference& cp);
		~MaterialReference();

		#ifndef IS_EDITOR
		static MaterialReference CreateNewMaterial();
		#endif
	};
}

#endif