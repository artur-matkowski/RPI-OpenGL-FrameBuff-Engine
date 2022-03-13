#include "Shader.hpp"
#include "Systems.hpp"
#include "bfu.hpp"

namespace asapi
{

	Shader::Shader()
	{}

	Shader* Shader::LoadShaderFromFile(const char* filename)
	{
		char buff[MAX_PATH_SIZE];
		SYSTEMS::IO::MMAP vert, frag;

		snprintf(buff, MAX_PATH_SIZE, "%s/assets_int/shaders/%s.vert.glsl", SYSTEMS::GetObject().ASSETS.GetProjectPath(), filename);
		vert.InitForRead(buff);

		snprintf(buff, MAX_PATH_SIZE, "%s/assets_int/shaders/%s.frag.glsl", SYSTEMS::GetObject().ASSETS.GetProjectPath(), filename);
		frag.InitForRead(buff);

		return RendererSystem::ProcessShader_obsolete((char*)vert.Data(), (char*)frag.Data(), filename);
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