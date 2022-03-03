#ifndef H_MaterialReference
#define H_MaterialReference

#include "SerializableObject.hpp"
#include "UniqueID.hpp"
#include "MaterialInstance.hpp"

namespace asapi
{
	class MaterialReference: public SerializableObject<MaterialReference>
	{
	protected:
		SERIALIZABLE_OBJ(MaterialReference, UniqueID, m_materialInstanceID);

		MaterialInstance* 	m_materialType = nullptr;
		uint16_t* 			m_referenceCounter = nullptr;


		MaterialReference();
	public:
		static MaterialReference LoadMaterial(const std::string projectPath, const UniqueID& uuid);
		MaterialReference(const MaterialReference& cp);
		~MaterialReference();

		inline UniqueID GetMaterialInstanceID(){ return m_materialInstanceID; }

		#ifndef IS_EDITOR
		static MaterialReference CreateNewMaterial();
		#endif
	};
}

#endif