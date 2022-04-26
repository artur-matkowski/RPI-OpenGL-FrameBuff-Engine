#ifndef H_ResourceGLSLSharedReference
#define H_ResourceGLSLSharedReference
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
	class ResourceGLSLProcessor: public ResourceProcessorBase<ResourceGLSLProcessor>
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
			asapi::FILE::MMAP* file = (asapi::FILE::MMAP*) handle;
			
			uint16_t* p_vertSize = (uint16_t*)file->Data();
			uint16_t* p_fragSize = &p_vertSize[1];
			uint16_t* p_shaderNameSize = &p_vertSize[2];
			char* vertex_source = (char*)&p_vertSize[3];
			char* fragment_source = vertex_source+*p_vertSize+1;
			char* p_nameBuff = fragment_source+*p_fragSize+1;

			ImGui::Text("Shader %s stats:", p_nameBuff);
			ImGui::Text("Vertex source:\n%s", vertex_source);
			ImGui::Text("Fragment source:\n%s", fragment_source);
		}
		#endif
	};


	class ResourceGLSLSharedReference: public asapi::ResourceSharedReferenceBase<ResourceGLSLSharedReference, ResourceGLSLProcessor>
	{
	public:
		ResourceGLSLSharedReference(){}

		ResourceGLSLSharedReference( ResourceGLSLSharedReference&& cp )
		{
			m_resourcePtr = cp.m_resourcePtr;
			m_binaryResourceID = std::move( cp.m_binaryResourceID );
			cp.m_resourcePtr = nullptr;
		}
	};

}

#endif