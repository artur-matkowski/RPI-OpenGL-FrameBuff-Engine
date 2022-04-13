#ifndef H_ResourcePNGSharedReference
#define H_ResourcePNGSharedReference
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
	class ResourcePNGProcessor: public ResourceProcessorBase<ResourcePNGProcessor>
	{
		static std::vector<std::string*> data;
	public:
		static void* LoadResource(const char* path);

		static void UnloadResource(void* handle);

		static bool ProcessResource2Binary(const asapi::ResourceTracker& in_currentResource
												, asapi::FILE::MMAP* in_resourceFile
												, const char* in_projectPath
												, std::vector<asapi::SubResourceData>* out_resourceBinaries);

		static const char* GetSuportedResourceFileExtension();

		static const char* GetBinaryOutputFileExtension();

		friend bfu::stream& operator<<(bfu::stream&, const ResourcePNGProcessor& );	

		#ifdef IS_EDITOR
		static void OnGUI(void* handle)
		{
			ImGui::Text( "Referenced Test Resource: %s", (char*)handle );

			for(int i=0; i<data.size(); i++)
				ImGui::Text( data[i]->c_str() );
		}
		#endif
	};


	class ResourcePNGSharedReference: public asapi::ResourceSharedReferenceBase<ResourcePNGSharedReference, ResourcePNGProcessor>
	{
	public:
		ResourcePNGSharedReference(){}

		ResourcePNGSharedReference( ResourcePNGSharedReference&& cp )
		{
			m_resourcePtr = cp.m_resourcePtr;
			cp.m_resourcePtr = nullptr;
		}
	};

}

#endif