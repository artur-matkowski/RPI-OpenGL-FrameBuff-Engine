#ifndef H_ResourceTXTSharedReference
#define H_ResourceTXTSharedReference
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

		#ifdef IS_EDITOR
		static void OnGUI(void* handle)
		{
			for(int i=0; i<data.size(); i++)
				ImGui::Text( data[i].c_str() );
			//ImGui::Text( "data[0].c_str()" );
		}
		#endif
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