#ifndef H_ResourceTXTSharedReference
#define H_ResourceTXTSharedReference
#include <vector>
#include <string>
#include "ResourceTrackerManager.hpp"
#include "ResourceSharedReference.hpp"
#include "ResourceProcessorBase.hpp"
#include "imgui.h"

namespace asapi
{
	class ResourceTXTProcessor: public ResourceProcessorBase<ResourceTXTProcessor>
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


		static void OnGUI(void* handle)
		{
			//ImGui::Text( data[0].c_str() );
			//ImGui::Text( "data[0].c_str()" );
		}
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

#endif