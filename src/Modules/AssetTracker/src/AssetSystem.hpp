#ifndef H_AssetSystem
#define H_AssetSystem
#include <vector>
#include "ResourceTracker.hpp"


namespace asapi
{
	class AssetSystem
	{
		char** 							ps_resourcesDirectoryPath = 0;
#ifdef TESTS
	public:
#endif
		std::vector<ResourceTracker> 	v_ResourceTrackers;
	public:
		void Init(char** resourcesPath);
		void RefreshResources();


		friend bfu::stream& operator<<(bfu::stream&, const AssetSystem& );

	};
}

#endif