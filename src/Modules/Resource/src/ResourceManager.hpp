#ifndef H_Resource_Manager
#define H_Resource_Manager
#include "ResourceBase.hpp"
#include "ResourceTrackerManager.hpp"

namespace asapi
{
	class ResourceManager
	{
		ResourceTrackerManager m_resourceTrackerManager;
	public:
		ResourceManager();
		~ResourceManager();
		
		void Init(const char* projectPath);
		void SetProjectPath(const char* projectPath);
		
		void RefreshResources();
	};
}

#endif