
#include <thread>

#include <xf86drm.h>
#include <xf86drmMode.h>
#include <gbm.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>


#include "Context.hpp"

#include <Systems.hpp>



namespace asapgl
{
	static const GLfloat vertices[] = {
	    -1.0f,
	    -1.0f,
	    0.0f,
	    1.0f,
	    -1.0f,
	    0.0f,
	    0.0f,
	    1.0f,
	    0.0f,
	};

	const EGLint attributes[] = {
	    EGL_BLUE_SIZE, 8, 
	    EGL_GREEN_SIZE, 8,
	    EGL_RED_SIZE, 8,

	    // Uncomment the following to enable MSAA
	    // EGL_SAMPLE_BUFFERS, 1, // <-- Must be set to 1 to enable multisampling!
	    // EGL_SAMPLES, 4, // <-- Number of samples

	    // Uncomment the following to enable stencil buffer
	    // EGL_STENCIL_SIZE, 1,

	    EGL_RENDERABLE_TYPE, 
	    EGL_OPENGL_ES2_BIT, 
	    EGL_NONE};
	    
	const EGLint contextAttribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE};


static void draw(const int & width, const int & height)
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

	glViewport(0, 0, width, height);
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
/*
void render()
{

    GLuint program, vert, frag, vbo;
    GLint posLoc, colorLoc, result;

	program = glCreateProgram();
    glUseProgram(program);
    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertexShaderCode, NULL);
    glCompileShader(vert);
    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragmentShaderCode, NULL);
    glCompileShader(frag);
    glAttachShader(program, frag);
    glAttachShader(program, vert);
    glLinkProgram(program);
    glUseProgram(program);

    // Create Vertex Buffer Object
    // Again, NO ERRRO CHECKING IS DONE! (for the purpose of this example)
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_DRAW);

    // Get vertex attribute and uniform locations
    posLoc = glGetAttribLocation(program, "pos");
    colorLoc = glGetUniformLocation(program, "color");

    // Set the desired color of the triangle to pink
    // 100% red, 0% green, 50% blue, 100% alpha
    glUniform4f(colorLoc, 1.0, 0.0f, 0.5, 1.0);

    // Set our vertex data
    glEnableVertexAttribArray(posLoc);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);

    // Render a triangle consisting of 3 vertices:
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
*/
	Context::Context()
		:context(0)
		,m_frameDelay(1.0/60.0)
	{

		m_isRunning = true;
	}

	Context::~Context()
	{
		CleanUp();

		if(context!=0)
			delete context;
	}

	void Context::CleanUp()
	{
		m_isRunning = false;
	}

	void Context::SetFPS(float FPS)
	{
		m_frameDelay = std::chrono::duration<double> (1.0/FPS);
	}



	#ifndef _TARGET
	void Context::initXlib(const int argc, const char** argv)
	{
		if(context==0)
		{
			bfu::EventSystem& events = SYSTEMS::GetObject().EVENTS;

			context = new Xlib_EGL_ContextType( attributes, contextAttribs, argc, argv );

			events.InitEvent<ResizeWindowArgs>("ResizeWindow");

			log::info << "GL initialized with version: " << glGetString(GL_VERSION) << std::endl;
			log::info << "GL vendor: " << glGetString(GL_VENDOR) << std::endl;
			log::info << "GL renderer: " << glGetString(GL_RENDERER) << std::endl;
			log::info << "GL shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
		}
		else
		{
			log::warning << "Context already initialized, skipping" << std::endl;
		}
	}
	#endif

	void Context::initDRM(const int argc, const char** argv)
	{
		if(context==0)
		{
			context = new DRM_GBM_EGL_ContextType( attributes, contextAttribs, argc, argv );

			log::info << "GL initialized with version: " << glGetString(GL_VERSION) << std::endl;
			log::info << "GL vendor: " << glGetString(GL_VENDOR) << std::endl;
			log::info << "GL renderer: " << glGetString(GL_RENDERER) << std::endl;
			log::info << "GL shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
		}
		else
		{
			log::warning << "Context already initialized, skipping" << std::endl;
		}
	}



	void Context::MainLoop()
	{
		bfu::EventSystem& events = SYSTEMS::GetObject().EVENTS;
		auto frameEnd =  std::chrono::system_clock::now();
		auto frameStart = std::chrono::high_resolution_clock::now();
    	bfu::CallbackId id;

		context->GetResolution(m_width, m_height);

		events.RegisterCallback<ResizeWindowArgs>(id, [&](bfu::EventArgsBase& a)
	    {
		    ResizeWindowArgs* args = (ResizeWindowArgs*)&a;
	    	m_width = args->m_width; 
	    	m_height = args->m_height; 
			log::debug << "resolution update invoked " << m_width << "x" << m_height  << std::endl;
	    });


		std::chrono::duration<double> elapsed;

		GLfloat rotation = 0.0;
		while(m_isRunning)
		{
			std::chrono::duration<double> frameDeltaTime = frameEnd - frameStart;
			frameStart = std::chrono::high_resolution_clock::now();

			context->HandleContextEvents();

			//TODO frame stuff
			{
				rotation += frameDeltaTime.count();
				glClearColor(1.0,0.0, 0.0, 0.0);
				glClear(GL_COLOR_BUFFER_BIT);


				//render();
				draw(m_width, m_height);

				context->SwapBuffer();
			}



			std::chrono::duration<double> calculationTime = std::chrono::high_resolution_clock::now() - frameStart;
			std::chrono::duration<double> diffToFrameEnd = m_frameDelay - calculationTime;


			//log::debug << "frameDeltaTime: "  << (float)frameDeltaTime.count() << "s, Calculation time: " << (float)calculationTime.count() << "s" << std::endl;

			std::this_thread::sleep_for(diffToFrameEnd);

			frameEnd = std::chrono::high_resolution_clock::now();
		}
		
	}
	

}