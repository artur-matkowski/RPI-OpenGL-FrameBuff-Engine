#include "Shader.hpp"
#include <bitforge/utils/bfu.hpp>

namespace asapgl
{
	struct
	{
		const char *vertex_source =
			"#version 100\n"
			"attribute vec4 position;\n"
			"attribute vec4 color;\n"
			"attribute vec2 texCoord;\n"
			"\n"
			"varying vec4 vcolor;\n"
			"varying vec2 UV;\n"
			"\n"
			"void main()\n"
			"{\n"
			"  gl_Position = position;\n"
			"  vcolor = color;\n"
			"  UV = texCoord;\n"
			"}\n";

		const char *fragment_source =
			"#version 100\n"
			"precision mediump float;\n"
			"varying vec4 vcolor;\n"
			"varying vec2 UV;\n"
			"uniform sampler2D texUnit;\n"
			"uniform float blend;\n"
			"\n"
			"void main()\n"
			"{\n"
			//"  gl_FragColor = texture2D(texUnit, UV) * vcolor;\n"
			"  gl_FragColor = texture2D(texUnit, UV) * vcolor * (blend+1.0);\n"
			//"  gl_FragColor = UV.xyyy * blend;\n"
			"}\n";
	}static debugShaderSrc;
		
	struct
	{
		const char *vertex_source =
			"#version 100\n"
			"attribute vec4 position;\n"
			"attribute vec4 color;\n"
			"attribute vec2 texCoord;\n"
			"uniform vec3 offset;\n"
			"\n"
			"varying vec4 vcolor;\n"
			"varying vec2 UV;\n"
			"\n"
			"void main()\n"
			"{\n"
			"  gl_Position = position + vec4(offset, 0.0);\n"
			"  vcolor = color;\n"
			"  UV = texCoord;\n"
			"}\n";

		const char *fragment_source =
			"#version 100\n"
			"precision mediump float;\n"
			"varying vec4 vcolor;\n"
			"varying vec2 UV;\n"
			"uniform sampler2D texUnit;\n"
			"uniform float blend;\n"
			"uniform vec3 offset;\n"
			"\n"
			"void main()\n"
			"{\n"
			//"  gl_FragColor = texture2D(texUnit, UV) * vcolor;\n"
			"  gl_FragColor = vec4(1.0) * (blend+1.0);\n"
			//"  gl_FragColor = UV.xyyy * blend;\n"
			"}\n";
	}static cursorShaderSrc;


	Shader::Shader(const char* filename)
	{
		char* vertex_source = 0;
		char* fragment_source = 0;
		GLuint vertex, fragment;
		GLint isCompiled = GL_FALSE;
		
		if( strcmp(filename, "cursor") == 0 )
		{
			vertex_source = (char*)cursorShaderSrc.vertex_source;
			fragment_source = (char*)cursorShaderSrc.fragment_source;
		}
		else
		{
			vertex_source = (char*)debugShaderSrc.vertex_source;
			fragment_source = (char*)debugShaderSrc.fragment_source;
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
			return;
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
			return;
		}



		m_programID = glCreateProgram();

		glAttachShader(m_programID, vertex);
		glAttachShader(m_programID, fragment);
		
		glBindAttribLocation(m_programID, 0, "position");
		glBindAttribLocation(m_programID, 1, "color");
		glBindAttribLocation(m_programID, 2, "texCoord");

		glLinkProgram(m_programID);

		glGetProgramiv(m_programID, GL_LINK_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);

			std::string str(&errorLog[0]);

			log::error << str << std::endl;

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteProgram(m_programID); // Don't leak the shader.
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_programID, vertex);
		glDetachShader(m_programID, fragment);

		glDeleteShader(vertex);
		glDeleteShader(fragment);
/*
		glGetProgramiv(m_programID, GL_VALIDATE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);

			std::string str(&errorLog[0]);

			log::error << str << std::endl;

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteProgram(m_programID); // Don't leak the shader.
			return;
		}
*/
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_programID);
	}
}