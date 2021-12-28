#include "ResourceReference.hpp"
#include "ResourceSystem.hpp"


namespace asapi
{
	std::string IResourceReferenceBase::s_projectPath;
	ResourceSystemBase* IResourceReferenceBase::s_resourceSystem = nullptr;

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
}