#ifndef H_ResourceFBXProcessorReference
#define H_ResourceFBXProcessorReference
#include <vector>
#include <string>
#include "ResourceTrackerManager.hpp"
#include "ResourceSharedReference.hpp"
#include "ResourceProcessorBase.hpp"
#ifdef IS_EDITOR
#include "imgui.h"
#endif

namespace asapi
{
	class ResourceFBXProcessor: public ResourceProcessorBase<ResourceFBXProcessor>
	{
	public:
		static void* LoadResource(const char* path);

		static void UnloadResource(void* handle);

		static bool ProcessResource2Binary(const asapi::ResourceTracker& in_currentResource
												, asapi::FILE::MMAP* in_resourceFile
												, const char* in_projectPath
												, std::vector<asapi::SubResourceData>* out_resourceBinaries);

		static const char* GetSuportedResourceFileExtension();

		static const char* GetBinaryOutputFileExtension();

		#ifdef IS_EDITOR
		static void OnGUI(void* handle)
		{
			
		}
		#endif
	};


	class ResourceFBXSharedReference: public asapi::ResourceSharedReferenceBase<ResourceFBXSharedReference, ResourceFBXProcessor>
	{
	public:
		ResourceFBXSharedReference(){}

		ResourceFBXSharedReference( ResourceFBXSharedReference&& cp )
		{
			m_resourcePtr = cp.m_resourcePtr;
			m_binaryResourceID = std::move( cp.m_binaryResourceID );
			cp.m_resourcePtr = nullptr;
		}
	};

}

#endif