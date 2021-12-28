#include <vector>
#include <string>
#include "ResourceTrackerManager.hpp"
#include "ResourceSharedReference.hpp"

namespace asapi
{
	class ResourceTXTProcessor
	{
		static std::vector<std::string> data;
	public:
		static void* LoadResource(const char* path);

		static void UnloadResource(void* handle);

		static bool ProcessResource2Binary(const asapi::ResourceTracker& in_currentResource
												, asapi::FILE::MMAP* in_resourceFile
												, const char* in_projectPath
												, std::vector<asapi::SubResourceData>* out_resourceBinaries);

		static const char* GetSuportedResourceFileExtension();

		static const char* GetBinaryOutputFileExtension();

		friend bfu::stream& operator<<(bfu::stream&, const ResourceTXTProcessor& );	
	};


	class ResourceTXTSharedReference: public asapi::ResourceSharedReferenceBase<ResourceTXTSharedReference, ResourceTXTProcessor>
	{
	public:
		ResourceTXTSharedReference(){}

		ResourceTXTSharedReference( ResourceTXTSharedReference&& cp )
		{
			m_resourcePtr = cp.m_resourcePtr;
			cp.m_resourcePtr = nullptr;
		}
	};
}