#include "MaterialReference.hpp"
#include "Systems.hpp"

namespace asapi
{
	void MaterialReference::CleanUp()
	{
		if( m_referenceCounter == nullptr )
			return;

		--(*m_referenceCounter);

		if(*m_referenceCounter==1)
		{
			SYSTEMS::GetObject().MATERIALSSYSTEM.DispouseMaterialReference( *this );
		}
		else if(*m_referenceCounter==0)
		{
			delete m_referenceCounter;
			delete m_materialInstance;
			m_referenceCounter = nullptr;
			m_materialInstance = nullptr;
		}
	}

	MaterialReference::MaterialReference()
	{
	}

	MaterialReference::~MaterialReference()
	{
		CleanUp();
	}
	bool MaterialReference::LoadMaterialInstance(const UniqueID& uuid)
	{
		CleanUp();

		m_referenceCounter = new uint16_t;
		*m_referenceCounter = 0;

		m_materialInstance = new MaterialInstance(uuid);
		m_materialInstanceID = uuid;

		return true;
	}

	MaterialReference& MaterialReference::operator=(const MaterialReference& cp)
	{
		CleanUp();

		m_referenceCounter = cp.m_referenceCounter;
		m_materialInstance = cp.m_materialInstance;
		m_materialInstanceID = cp.m_materialInstanceID;

		(*m_referenceCounter)++;

		return *this;
	}

	MaterialReference::MaterialReference(const MaterialReference& cp)
	{
		*this = cp;
	}

	void MaterialReference::PostDeserializationCallback()
	{
		SYSTEMS::GetObject().MATERIALSSYSTEM.UpdateMaterialReference(m_materialInstanceID, this);
	}

	#ifdef IS_EDITOR
	MaterialReference MaterialReference::CreateNewMaterial()
	{
		return MaterialReference();
	}
	
	void MaterialReference::OnGUI()
	{
		SYSTEMS::GetObject().MATERIALSSYSTEM.SelectMaterialReference(this);

		if( m_materialInstance!=nullptr )
		{
			m_materialInstance->OnGUI_caller();
		}
	}
	#endif
}