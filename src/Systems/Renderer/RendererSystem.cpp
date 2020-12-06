#include <xf86drm.h>
#include <xf86drmMode.h>
#include <gbm.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "RendererSystem.hpp"
#include "ContextBase.hpp"
#include "Systems.hpp"



namespace asapgl{


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
		"attribute vec4 position;\n"
		"attribute vec4 color;\n"
		"\n"
		"varying vec4 vcolor;\n"
		"\n"
		"void main()\n"
		"{\n"
		"  gl_Position = position;\n"
		"  vcolor = color;\n"
		"}\n";

	static const char *fragment_source =
		"precision mediump float;\n"
		"varying vec4 vcolor;\n"
		"\n"
		"void main()\n"
		"{\n"
		"  gl_FragColor = vcolor;\n"
		"}\n";

	static GLfloat vertices[] = {
		 0.0f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
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

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);

	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "color");


	glLinkProgram(program);
	glFlush();

	glUseProgram(program);
	glFlush();

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

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glFlush();


	}
			
	

}