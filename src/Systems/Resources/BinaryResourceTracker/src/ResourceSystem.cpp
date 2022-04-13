#include "ResourceSystem.hpp"
#include "ResourceReference.hpp"

namespace asapi
{
	void ResourceSystemBase::Init()
	{
		IResourceReferenceBase::SetResourceSystemReference( this );
		ResourceSharedReferenceInterface::SetResourceSystemReference( this );
	}
	

	#ifdef IS_EDITOR
	void ResourceSystemBase::RefreshResourceReferences()
	{
		IResourceReferenceBase::RefreshResourceReference();
	}
	#endif
}