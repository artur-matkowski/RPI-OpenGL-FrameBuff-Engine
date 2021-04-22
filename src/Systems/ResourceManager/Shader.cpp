#include "Shader.hpp"
#include "Systems.hpp"
#include <bitforge/utils/bfu.hpp>

namespace asapi
{

	Shader::Shader()
	{}

	Shader* Shader::LoadShaderFromFile(const char* filename)
	{
		char buff[MAX_PATH_SIZE];
		char vertex_source[1024*1024*1];
		char fragment_source[1024*1024*1];
		FILE * pFile = nullptr;
		long int fileSize = 0;

		

		sprintf(buff, "%s/assets_int/shaders/%s.vert.glsl", SYSTEMS::GetObject().RESOURCES.GetProjectPath(), filename);

		SYSTEMS::IO::OpenFile(&pFile, &fileSize, buff);
		ASSERT( 1024*1024*1<fileSize, "shader file exceeds 1024*1024*1 bytes");
		if( fileSize!=-1 )
		{
			SYSTEMS::IO::ReadFile(pFile, vertex_source, fileSize);
			vertex_source[fileSize] = '\0';
			SYSTEMS::IO::CloseFile(pFile);
		}


		sprintf(buff, "%s/assets_int/shaders/%s.frag.glsl", SYSTEMS::GetObject().RESOURCES.GetProjectPath(), filename);

		SYSTEMS::IO::OpenFile(&pFile, &fileSize, buff);
		ASSERT( 1024*1024*1<fileSize, "shader file exceeds 1024*1024*1 bytes");
		if( fileSize!=-1 )
		{
			SYSTEMS::IO::ReadFile(pFile, fragment_source, fileSize);
			fragment_source[fileSize] = '\0';
			SYSTEMS::IO::CloseFile(pFile);
		}


		Shader* ret = new Shader();
		ret->vertex_source = vertex_source;
		ret->fragment_source = fragment_source;
		ret->shaderName = filename;

		RendererSystem::ProcessShader(ret);
		return ret;
	}

	Shader::~Shader()
	{
		RendererSystem::DispouseShader(this);
	}

	void Shader::Compile(const char* dest, const char* source)
	{
		FILE *src, *dst;
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