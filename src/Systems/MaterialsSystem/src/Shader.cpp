#include "Shader.hpp"
#include "Systems.hpp"
#include "bfu.hpp"
#include "File.hpp"

namespace asapi
{
	Shader::Shader()
	{}

	Shader* Shader::LoadShaderFromFile(const char* filename)
	{
		char buff[MAX_PATH_SIZE];
		FILE::MMAP vert, frag;

		snprintf(buff, MAX_PATH_SIZE, "%s/assets_int/shaders/%s.vert.glsl", ResourceSystemBase::GetProjectPath(), filename);
		vert.InitForRead(buff);

		snprintf(buff, MAX_PATH_SIZE, "%s/assets_int/shaders/%s.frag.glsl", ResourceSystemBase::GetProjectPath(), filename);
		frag.InitForRead(buff);

		return RendererSystem::ProcessShader_obsolete((char*)vert.Data(), (char*)frag.Data(), filename);
	}

	bool Shader::LoadShaderFromResourceRawHandle(const void* resourceHandle)
	{		
		asapi::FILE::MMAP* file = (asapi::FILE::MMAP*) resourceHandle;
			
		uint16_t* p_vertSize = (uint16_t*)file->Data();
		uint16_t* p_fragSize = &p_vertSize[1];
		uint16_t* p_shaderNameSize = &p_vertSize[2];
		char* vertex_source = (char*)&p_vertSize[3];
		char* fragment_source = vertex_source+*p_vertSize+1;
		char* p_nameBuff = fragment_source+*p_fragSize+1;

		if(IsValid())
			RendererSystem::DispouseShader_obsolete(this);

		RendererSystem::ProcessShader_obsolete(vertex_source, fragment_source, p_nameBuff, this);

		return IsValid();
	}

	Shader::~Shader()
	{
		RendererSystem::DispouseShader_obsolete(this);
	}

	void Shader::Compile(const char* dest, const char* source)
	{
		::FILE *src, *dst;
		long int srcSize, dstSize;

		src = fopen (source,"rb");
		fseek(src, 0L, SEEK_END); 
		srcSize = ftell(src); 
		fseek(src, 0L, SEEK_SET);

		char* buff = new char[srcSize];

		fread(buff, sizeof(char), srcSize, src);

		delete buff;

		fclose(src); 


		dst = fopen (dest,"wb");
		fwrite(buff, 1, srcSize, dst);
		fwrite(" \n ", 1, 3, dst);
		fclose(dst); 
	}
}