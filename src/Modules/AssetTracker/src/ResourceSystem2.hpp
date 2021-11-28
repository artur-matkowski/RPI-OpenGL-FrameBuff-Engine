#ifndef H_RESOURCESYSTEM
#define H_RESOURCESYSTEM
#include <vector>
#include "ResourceTracker.hpp"


namespace asapi
{
	class ResourceSystem2
	{
		char** 							ps_resourcesDirectoryPath = 0;
		std::vector<ResourceTracker> 	v_ResourceTrackers;
	public:
		void Init(char** resourcesPath);
		void RefreshResources();
	};
}

#endif