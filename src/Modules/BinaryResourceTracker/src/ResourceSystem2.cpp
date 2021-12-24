#include "ResourceSystem2.hpp"


namespace asapi
{
	//std::string ResourceSystemBase::s_projectPath;

	void ResourceSystemBase::Init()
	{
		IResourceReferenceBase::SetResourceSystemReference( this );
		ResourceSharedReferenceInterface::SetResourceSystemReference( this );
	}

	void ResourceSystemBase::SetProjectPath(const char* projectPath)
	{
		s_projectPath = projectPath;
		IResourceReferenceBase::SetProjectPath( projectPath );
	}
		
	BinaryResourceTracker* ResourceSystemBase::RequestBinaryResourceTracker( UniqueID id )
	{
		return m_binaryResourceTrackers[id];
	}
}