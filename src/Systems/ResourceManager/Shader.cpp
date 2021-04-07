#include "Shader.hpp"
#include "Systems.hpp"
#include <bitforge/utils/bfu.hpp>

namespace asapi
{	
	struct
	{
		const char *vertex_source =
			"#version 100\n"
			"precision mediump float;\n"
			"attribute vec4 position;\n"
			"attribute vec4 color;\n"
			"attribute vec2 texCoord;\n"
			"uniform vec3 offset;\n"
			"uniform mat4 modelViewMat;\n"
			"\n"
			"varying vec4 vcolor;\n"
			"varying vec2 UV;\n"
			"\n"
			"void main()\n"
			"{\n"
			"  gl_Position = modelViewMat * position;\n"
			"  UV = texCoord;\n"
			"}\n";

		const char *fragment_source =
			"#version 100\n"
			"precision mediump float;\n"
			"varying vec4 vcolor;\n"
			"varying vec2 UV;\n"
			"uniform sampler2D texUnit;\n"
			"\n"
			"void main()\n"
			"{\n"
			"  gl_FragColor = vec4(1.0);\n"
			"}\n";
	}static debugShaderSrc;

	Shader::Shader(uint32_t id)
		:m_programID(id)
	{}

	Shader* Shader::LoadShader(const char* filename)
	{
		char buff[1024];
		char* vertex_source = 0;
		char* fragment_source = 0;
		GLuint vertex, fragment;
		GLint isCompiled = GL_FALSE;
		uint32_t programID = -1;
		Shader* ret = nullptr;
		bool isDebug = strcmp(filename, "debug") == 0;
		

		if( isDebug )
		{
			vertex_source = (char*)debugShaderSrc.vertex_source;
		}
		else
		{
			sprintf(buff, "%s/shaders/%s.vert.glsl", SYSTEMS::GetObject().SCENE.GetProjectPath(), filename);
			FILE * pFile = fopen (buff,"rb");

			fseek(pFile, 0L, SEEK_END); 
			auto fileSize = ftell(pFile); 
			fseek(pFile, 0L, SEEK_SET);

			vertex_source = new char[fileSize];
			fread(vertex_source, sizeof(char), fileSize, pFile);

			fclose (pFile);
		}

		vertex = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertex, 1, &vertex_source, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(vertex, maxLength, &maxLength, &errorLog[0]);

			std::string str(&errorLog[0]);

			log::error << str << std::endl;

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(vertex); // Don't leak the shader.
			return nullptr;
		}




		if( isDebug )
		{
			fragment_source = (char*)debugShaderSrc.fragment_source;
		}
		else
		{
			sprintf(buff, "%s/shaders/%s.frag.glsl", SYSTEMS::GetObject().SCENE.GetProjectPath(), filename);
			FILE * pFile = fopen (buff,"rb");

			fseek(pFile, 0L, SEEK_END); 
			auto fileSize = ftell(pFile); 
			fseek(pFile, 0L, SEEK_SET);

			fragment_source = new char[fileSize];
			fread(fragment_source, sizeof(char), fileSize, pFile);

			fclose (pFile);
		}

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragment_source, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(fragment, maxLength, &maxLength, &errorLog[0]);

			std::string str(&errorLog[0]);

			log::error << str << std::endl;

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(fragment); // Don't leak the shader.
			return nullptr;
		}



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

		ret = new Shader(programID);

		glDeleteShader(vertex);
		glDeleteShader(fragment);

		return ret;
/*
		glGetProgramiv(programID, GL_VALIDATE_STATUS, &isCompiled);
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
			return;
		}
*/
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_programID);
	}
}