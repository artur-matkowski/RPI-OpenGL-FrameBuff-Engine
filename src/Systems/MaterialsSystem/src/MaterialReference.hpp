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

		MaterialInstance* 	m_materialInstance = nullptr;
		uint16_t* 			m_referenceCounter = nullptr;

		void CleanUp();
	public:
		MaterialReference();
		MaterialReference& operator=(const MaterialReference& cp);
		MaterialReference(const MaterialReference& cp);
		~MaterialReference();


		bool LoadMaterialInstance(const UniqueID& uuid);

		inline UniqueID GetMaterialInstanceID(){ return m_materialInstanceID; }
		inline const int GetReferencesCount() const { return *m_referenceCounter; }
		inline MaterialInstance* GetMaterialInstance() { return m_materialInstance; }

		virtual void PostDeserializationCallback() override;


		bool operator==(const MaterialReference& cp) const;

		#ifdef IS_EDITOR
		static MaterialReference CreateNewMaterial();
		virtual void OnGUI() override;
		#endif
	};
}

#endif