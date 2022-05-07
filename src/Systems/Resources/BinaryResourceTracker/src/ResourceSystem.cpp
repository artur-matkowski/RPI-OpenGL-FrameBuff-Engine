#include "ResourceSystem.hpp"
#include "ResourceReference.hpp"

namespace asapi
{
	std::string ResourceSystemBase::s_projectPath;

	
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