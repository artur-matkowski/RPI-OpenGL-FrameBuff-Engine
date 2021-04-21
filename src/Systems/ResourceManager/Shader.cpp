#include "Shader.hpp"
#include "Systems.hpp"
#include <bitforge/utils/bfu.hpp>

namespace asapi
{	
#define VERTEX_SOURCE  \
			"#version 100\n" \
			"precision mediump float;\n" \
			"attribute vec4 position;\n" \
			"attribute vec4 color;\n" \
			"attribute vec2 texCoord;\n" \
			"uniform vec3 offset;\n" \
			"uniform mat4 modelViewMat;\n" \
			"\n" \
			"varying vec4 vcolor;\n" \
			"varying vec2 UV;\n" \
			"\n" \
			"void main()\n" \
			"{\n" \
			"  gl_Position = modelViewMat * position;\n" \
			"  UV = texCoord;\n" \
			"}\n"

#define FRAGMENT_SOURCE  \
			"#version 100\n" \
			"precision mediump float;\n" \
			"varying vec4 vcolor;\n" \
			"varying vec2 UV;\n" \
			"uniform sampler2D texUnit;\n" \
			"\n" \
			"void main()\n" \
			"{\n" \
			"  gl_FragColor = vec4(1.0);\n" \
			"}\n"


	GLuint LoadSingleShader(const char* source, GLenum shaderType, const char* filename)
	{
		char b_source[1024*1024*1];
		GLint isCompiled = GL_FALSE;

		GLuint shader = glCreateShader(shaderType);

		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

			std::string str(&errorLog[0]);

			log::error << "In shader: " << filename << "\n\t" << str << std::endl;

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(shader); // Don't leak the shader.
			return 0;
		}

		return shader;
	}

	Shader::Shader(uint32_t id)
		:m_programID(id)
	{}

	Shader* Shader::LoadShaderFromFile(const char* filename)
	{
		char buff[1024*1];
		char vertex_source[1024*1024*1];
		char fragment_source[1024*1024*1];
		GLuint vertex, fragment;
		GLint isCompiled = GL_FALSE;
		uint32_t programID = -1;
		Shader* ret = nullptr;
		FILE * pFile = nullptr;
		long int fileSize = 0;

		

		sprintf(buff, "%s/assets_int/shaders/%s.vert.glsl", SYSTEMS::GetObject().RESOURCES.GetProjectPath(), filename);

		SYSTEMS::IO::OpenFile(&pFile, &fileSize, buff);
		ASSERT( 1024*1024*1<fileSize, "shader file exceeds 1024*1024*1 bytes");
		if( fileSize==-1 )
			return nullptr;
		SYSTEMS::IO::ReadFile(pFile, vertex_source, fileSize);
		vertex_source[fileSize] = '\0';
		SYSTEMS::IO::CloseFile(pFile);


		sprintf(buff, "%s/assets_int/shaders/%s.frag.glsl", SYSTEMS::GetObject().RESOURCES.GetProjectPath(), filename);

		SYSTEMS::IO::OpenFile(&pFile, &fileSize, buff);
		ASSERT( 1024*1024*1<fileSize, "shader file exceeds 1024*1024*1 bytes");
		if( fileSize==-1 )
			return nullptr;
		SYSTEMS::IO::ReadFile(pFile, fragment_source, fileSize);
		fragment_source[fileSize] = '\0';
		SYSTEMS::IO::CloseFile(pFile);
	


		vertex = LoadSingleShader(vertex_source, GL_VERTEX_SHADER, filename);
		if(vertex==0)
			return nullptr;


		fragment = LoadSingleShader(fragment_source, GL_FRAGMENT_SHADER, filename);
		if(fragment==0)
			return nullptr;


		return LinkShader(vertex, fragment);
	}

	Shader* Shader::LoadShaderFromSource(const char* vertex_source, const char* fragment_source, const char* filename)
	{
		GLuint vertex, fragment;
		GLint isCompiled = GL_FALSE;
		uint32_t programID = -1;
		Shader* ret = nullptr;
		

		vertex = LoadSingleShader(vertex_source, GL_VERTEX_SHADER, filename);
		if(vertex==0)
			return nullptr;


		fragment = LoadSingleShader(fragment_source, GL_FRAGMENT_SHADER, filename);
		if(fragment==0)
			return nullptr;


		return LinkShader(vertex, fragment);
	}

	Shader* Shader::LoadShaderFailSave()
	{
		return LoadShaderFromSource(VERTEX_SOURCE, FRAGMENT_SOURCE, "-failsave shader-");
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_programID);
	}

	Shader* Shader::LinkShader(GLuint vertex, GLuint fragment)
	{
		GLint isCompiled = GL_FALSE;
		uint32_t programID = -1;


		programID = glCreateProgram();

		glAttachShader(programID, vertex);
		glAttachShader(programID, fragment);
		
		glBindAttribLocation(programID, 0, "position");
		glBindAttribLocation(programID, 1, "color");
		glBindAttribLocation(programID, 2, "texCoord");

		glLinkProgram(programID);

		glGetProgramiv(programID, GL_LINK_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetProgramInfoLog(programID, maxLength, &maxLength, &errorLog[0]);

			std::string str(&errorLog[0]);

			log::error << str << std::endl;

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteProgram(programID); // Don't leak the shader.
			return nullptr;
		}

		// Always detach shaders after a successful link.
		glDetachShader(programID, vertex);
		glDetachShader(programID, fragment);

		Shader* ret = new Shader(programID);

		glDeleteShader(vertex);
		glDeleteShader(fragment);

		return ret;
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