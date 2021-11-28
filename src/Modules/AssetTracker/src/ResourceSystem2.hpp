#ifndef H_RESOURCESYSTEM
#define H_RESOURCESYSTEM

namespace asapi
{
	class ResourceSystem2
	{
		char** ps_resourcesDirectoryPath = 0;
	public:
		void Init(char** resourcesPath);
		void RefreshResources();
	};
}

#endif