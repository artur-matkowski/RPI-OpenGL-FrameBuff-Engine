#ifndef H_Resource_Manager
#define H_Resource_Manager
#include "ResourceBase.hpp"
#include "ResourceTrackerManager.hpp"
#include "Texture.hpp"
#include "Shaders.hpp"
#include "Mesh.hpp"
#include <map>

namespace asapi
{
	class ResourceManager
	{
		ResourceTrackerManager m_resourceTrackerManager;

		std::map<uint64_t, ResourceBase* > m_textures;
	public:
		ResourceManager();
		~ResourceManager();
		
		void Init(const char* projectPath);
		void SetProjectPath(const char* projectPath);
		
		void RefreshResources();

		ResourceBase RequestTexture(uint64_t resourceID);
	};
}

#endif