#include "Shader.hpp"
#include <bitforge/utils/bfu.hpp>

namespace asapgl
{
	Shader::Shader(const char* filename)
	{

		static const char *vertex_source =
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

		static const char *fragment_source =
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
			"  gl_FragColor = texture2D(texUnit, UV) * vcolor * blend;\n"
			//"  gl_FragColor = UV.xyyy * blend;\n"
			"}\n";

		GLuint vertex, fragment, program;

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertex_source, NULL);
		glCompileShader(vertex);

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragment_source, NULL);
		glCompileShader(fragment);

		GLint isCompiled = 0;
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
		}



		m_programID = glCreateProgram();

		glAttachShader(m_programID, vertex);
		glAttachShader(m_programID, fragment);
		
		glBindAttribLocation(program, 0, "position");
		glBindAttribLocation(program, 1, "color");
		glBindAttribLocation(program, 2, "texCoord");

		glLinkProgram(m_programID);

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	Shader::~Shader()
	{
		glDeleteShader(m_programID);
	}
}