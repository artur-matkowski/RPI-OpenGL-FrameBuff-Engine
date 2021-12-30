#include "ResourceSystem.hpp"


namespace asapi
{
	void ResourceSystemBase::Init()
	{
		IResourceReferenceBase::SetResourceSystemReference( this );
		ResourceSharedReferenceInterface::SetResourceSystemReference( this );
	}
	
}