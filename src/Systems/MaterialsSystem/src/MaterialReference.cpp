#include "MaterialReference.hpp"
#include "Systems.hpp"

namespace asapi
{
	MaterialReference MaterialReference::LoadMaterial(const std::string projectPath, const UniqueID& uuid)
	{
		MaterialReference ret;

		return ret;
	}

	MaterialReference::MaterialReference()
	{
		m_referenceCounter = new uint16_t;
		*m_referenceCounter = 0;

	}

	MaterialReference::~MaterialReference()
	{
		--(*m_referenceCounter);

		if(*m_referenceCounter==1)
		{
			SYSTEMS::GetObject().MATERIALSSYSTEM.DispouseMaterialReference( *this );
		}
		else if(*m_referenceCounter==0)
		{
			delete m_referenceCounter;
			delete m_materialType;
		}
	}

	MaterialReference::MaterialReference(const MaterialReference& cp)
	{
		m_referenceCounter = cp.m_referenceCounter;
		m_materialType = cp.m_materialType;

		(*m_referenceCounter)++;
	}

	#ifndef IS_EDITOR
	MaterialReference MaterialReference::CreateNewMaterial()
	{
		return MaterialReference();
	}
	#endif
}