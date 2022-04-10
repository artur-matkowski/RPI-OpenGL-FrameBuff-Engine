#include "ResourceReference.hpp"
#include "ResourceSystem.hpp"


namespace asapi
{
	std::string IResourceReferenceBase::s_projectPath;
	ResourceSystemBase* IResourceReferenceBase::s_resourceSystem = nullptr;
	std::vector<IResourceReferenceBase*> IResourceReferenceBase::s_resourceReferences;

	BinaryResourceTracker* IResourceReferenceBase::RequestBinaryResourceTracker( UniqueID id )
	{
		return s_resourceSystem->RequestBinaryResourceTracker(id);
	}

	void IResourceReferenceBase::DecreaseReferenceCounter()
	{
		m_referenceCounter--;

		if( m_referenceCounter==0 )
		{
			s_resourceSystem->ScheduleGarbageCollection();
		}
	}

	void IResourceReferenceBase::IncreaseReferenceCounter()
	{
		m_referenceCounter++;
	}

	void IResourceReferenceBase::RefreshResourceReference()
	{
		log::debug << "IResourceReferenceBase::RefreshResourceReference()" << std::endl;

		for( auto it = IResourceReferenceBase::s_resourceReferences.begin(); it!=IResourceReferenceBase::s_resourceReferences.end(); it++ )
		{
			(*it)->ReloadResourceReference();
		}

	}
}