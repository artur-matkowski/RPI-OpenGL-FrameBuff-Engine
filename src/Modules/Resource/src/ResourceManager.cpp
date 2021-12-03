#include "ResourceManager.hpp"

namespace asapi
{

	ResourceManager::ResourceManager()
		:m_resourceTrackerManager()
	{

	}

	ResourceManager::~ResourceManager()
	{

	}
	void ResourceManager::Init(const char* projectPath)
	{
		m_resourceTrackerManager.Init( projectPath );
	}
	void ResourceManager::SetProjectPath(const char* projectPath)
	{
		m_resourceTrackerManager.SetProjectPath( projectPath );
	}

	void ResourceManager::RefreshResources()
	{
		m_resourceTrackerManager.RefreshResources();
		m_resourceTrackerManager.IterateOverDirtyResourceTrackers([](ResourceTracker* resourceTracker, void* resourceManager)
			{
				
				return false;
			}, this);
	}
}