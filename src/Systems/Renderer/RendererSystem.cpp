#include <xf86drm.h>
#include <xf86drmMode.h>
#include <gbm.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <png.h>

#include "RendererSystem.hpp"
#include "ContextBase.hpp"
#include "Systems.hpp"



namespace asapgl{

	void RendererSystem::Init()
	{
		texture = new Texture("debug.png");
	}

	RendererSystem::RendererSystem()
	{

	}
	RendererSystem::~RendererSystem()
	{

	}

	void RendererSystem::SetupEvents()
	{
    	bfu::CallbackId id;
		bfu::EventSystem& events = SYSTEMS::GetObject().EVENTS;

		events.RegisterCallback<ResizeWindowArgs>(id, [&](bfu::EventArgsBase& a)
	    {
		    ResizeWindowArgs* args = (ResizeWindowArgs*)&a;
	    	m_width = args->m_width; 
	    	m_height = args->m_height; 
			log::debug << "resolution update invoked on RendererSystem: " << m_width << "x" << m_height  << std::endl;
	    });
	}


	void RendererSystem::Render()
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

		static GLfloat vertices[] = {
			 0.0f,  0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
		};

		static GLfloat texCoords[] = {
			 0.0f,  0.0f,
			 0.0f,  1.0f,
			 1.0f,  1.0f, 
		};

		static GLfloat colors[] = {
			1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
		};

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



		program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);

		glBindAttribLocation(program, 0, "position");
		glBindAttribLocation(program, 1, "color");
		glBindAttribLocation(program, 2, "texCoord");

		//BindTexture();


		glLinkProgram(program);
		glFlush();

		glUseProgram(program);
		glFlush();

		GLint texUnitLoc = glGetUniformLocation(program, "texUnit");
		GLint blendLoc = glGetUniformLocation(program, "blend");

		log::debug << "texUnitLoc: " << texUnitLoc << std::endl;
		log::debug << "blendLoc: " << blendLoc << std::endl;
		log::debug << "m_textureID: " << m_textureID << std::endl;

		texture->BindTexture();
		glUniform1i(texUnitLoc , 0);
		glUniform1f(blendLoc , 1.0);

		glViewport(0, 0, m_width, m_height);
		glFlush();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glFlush();

		glClear(GL_COLOR_BUFFER_BIT);
		glFlush();


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
		glFlush();

		glEnableVertexAttribArray(0);
		glFlush();

		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, colors);
		glFlush();

		glEnableVertexAttribArray(1);
		glFlush();

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
		glFlush();

		glEnableVertexAttribArray(2);
		glFlush();

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glFlush();

	}
			
	

}