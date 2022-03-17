#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "ResourceGLSLProcessor.hpp"
#include "ResourceSystem.hpp"
#include "RendererSystem.hpp"
#include "bfu.hpp"

namespace asapi
{
	void* ResourceGLSLProcessor::LoadResource(const char* path)
	{
		asapi::FILE::MMAP* file = new asapi::FILE::MMAP();

		file->InitForRead( path );

		if( !file->IsValid() )
		{
			log::error << "can not open resource binary: " << path << std::endl;
			return nullptr;
		}

		//Pointers Interpretations:
		/*
		uint16_t* p_vertSize = (uint16_t*)file.Data();
		uint16_t* p_fragSize = &p_vertSize[1];
		uint16_t* p_shaderNameSize = &p_vertSize[2];
		char* vertex_source = (char*)&p_vertSize[3];
		char* fragment_source = vertex_source+*p_vertSize+1;
		char* p_nameBuff = fragment_source+*p_fragSize+1;
		*/


		return (void*)file;
	}

	void ResourceGLSLProcessor::UnloadResource(void* handle)
	{
		asapi::FILE::MMAP* file = (asapi::FILE::MMAP*) handle;
		delete file;
	}


	bool ResourceGLSLProcessor::ProcessResource2Binary(const asapi::ResourceTracker& in_currentResource
											, asapi::FILE::MMAP* in_resourceFile
											, const char* in_projectPath
											, std::vector<asapi::SubResourceData>* out_resourceBinaries)
	{
		std::string subResourceDir = in_projectPath;
		subResourceDir += RESOURCE_BINARIES_DIR;
		subResourceDir += "/";

		out_resourceBinaries->clear();
		
		asapi::FILE::MMAP _out;
		asapi::UniqueID subresourceID;
		std::string subResourcePath;

		bool subresourcePreviouslyExisted = in_currentResource.FindSubResourceByInternalID( std::string( in_currentResource.GetFilename() ), subresourceID );

		char* vertBuff = new char[ in_resourceFile->Size() ];
		char* fragBuff = new char[ in_resourceFile->Size() ];
		std::string shaderName = in_currentResource.GetPath();


		vertBuff = strstr( (char*)in_resourceFile->Data(), "##VERT:\n") + strlen( "##VERT:\n" );
		fragBuff = strstr( (char*)in_resourceFile->Data(), "##FRAG:\n");
		uint16_t vertSize = fragBuff - vertBuff;
		fragBuff += strlen( "##FRAG:\n" );
		uint16_t fragSize = strlen(fragBuff);
		uint16_t shaderNameSize = (uint16_t) shaderName.length();


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//																								//
		//																								//
		//																								//
		//																								//
		//				This if section needs to be reimplemented in custom implementations.			//
		//				Otherwise binary resource name will not be tranfered between updates			//
		//																								//
		//																								//
		//																								//
		//																								//
		//////////////////////////////////////////////////////////////////////////////////////////////////

		subResourcePath = std::to_string( (uint64_t)subresourceID ) 
						+ std::string( ResourceGLSLProcessor::GetBinaryOutputFileExtension() )
						+ std::string(".bin");

		asapi::SubResourceData subresource(
								subresourceID
								, std::string( in_currentResource.GetFilename() ));

		subResourcePath = subResourceDir + subResourcePath;

		_out.InitForWrite( subResourcePath.c_str()
						, sizeof(uint16_t)*3 + (size_t)(vertSize+fragSize+shaderNameSize+3) );

		uint16_t* p_vertSize = (uint16_t*)_out.Data();
		uint16_t* p_fragSize = &p_vertSize[1];
		uint16_t* p_shaderNameSize = &p_vertSize[2];
		char* p_vertBuff = (char*)&p_vertSize[3];
		char* p_fragBuff = p_vertBuff+vertSize+1;
		char* p_nameBuff = p_fragBuff+fragSize+1;

		*p_vertSize = vertSize;
		*p_fragSize = fragSize;
		*p_shaderNameSize = shaderNameSize;
		strncpy(p_vertBuff, vertBuff, vertSize);
		strncpy(p_fragBuff, fragBuff, fragSize);
		strncpy(p_nameBuff, shaderName.c_str(), shaderNameSize);



		out_resourceBinaries->push_back( subresource );
		
		return true;
	}


	const char* ResourceGLSLProcessor::GetSuportedResourceFileExtension()
	{
		return ".glsl";
	}

	const char* ResourceGLSLProcessor::GetBinaryOutputFileExtension()
	{
		return ".glsl";
	}
}