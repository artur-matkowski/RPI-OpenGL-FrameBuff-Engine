#ifndef H_ResourceBase
#define H_ResourceBase
#include <stdint.h>
#include <string>

namespace asapi
{
	class ResourceManager;

	class ResourceBase
	{
		friend ResourceManager;

	private:
		uint64_t 		m_resourceIDlink = 0;
    	int* 			m_reference = 0;
    	
    	#ifdef IS_EDITOR
    	std::string 	m_resourceName;
    	std::string 	m_resourcePath;
    	#endif

	protected:
		virtual void LoadResource();
		virtual void DispouseResource();

		//for use only by Resource manager
		//reference counter is set to 0 as ResourceManager does not use the resource. so if only manager is holding to it, it should actually be dispoused
		ResourceBase(uint64_t resourceIDlink);
	public:
		ResourceBase(const ResourceBase& cp);
		virtual ~ResourceBase();
	};
}

#endif