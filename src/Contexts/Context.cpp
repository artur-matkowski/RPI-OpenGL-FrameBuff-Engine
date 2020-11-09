#include "Context.hpp"

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

// The following are GLSL shaders for rendering a triangle on the screen
#define STRINGIFY(x) #x
static const char *vertexShaderCode = STRINGIFY(
    attribute vec3 pos; void main() { gl_Position = vec4(pos, 1.0); });

static const char *fragmentShaderCode =
    STRINGIFY(uniform vec4 color; void main() { gl_FragColor = vec4(color); });

static void draw()
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

	printf("=== Calling glLinkProgram()\n");
	glLinkProgram(program);
	glFlush();
	printf("=== Calling glUseProgram()\n");
	glUseProgram(program);
	glFlush();
	printf("=== calling glViewport()\n");
	glViewport(0, 0, 1024, 600);
	glFlush();
	printf("=== calling glClearColor()\n");
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glFlush();
	printf("=== calling glClear()\n");
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();

	printf("=== Calling glVertexAttribPointer(0)\n");
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glFlush();
	printf("=== Calling glEnableVertexAttribArray(0)\n");
	glEnableVertexAttribArray(0);
	glFlush();
	printf("=== Calling glVertexAttribPointer(1)\n");
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, colors);
	glFlush();
	printf("=== Calling glEnableVertexAttribArray(1)\n");
	glEnableVertexAttribArray(1);
	glFlush();
	printf("=== Calling glDrawArrays(GL_TRIANGLES, 0, 3)\n");
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glFlush();
	printf("=== calling eglSwapBuffers()\n");
}

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

	Context::Context()
		:context(0)
		,m_frameDelay(1.0/60.0)
		,m_isRunning(true)
	{

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

	void Context::SetFPS(GLfloat FPS)
	{
		m_frameDelay = std::chrono::duration<double> (1.0/FPS);
	}

	void Context::MainLoop()
	{
		auto frameEnd =  std::chrono::system_clock::now();
		auto frameStart = std::chrono::high_resolution_clock::now();


		std::chrono::duration<double> elapsed;

		GLfloat rotation = 0.0;
		while(m_isRunning)
		{
			std::chrono::duration<double> frameDeltaTime = frameEnd - frameStart;
			frameStart = std::chrono::high_resolution_clock::now();

			

			//TODO frame stuff
			{
				rotation += frameDeltaTime.count();
				glClearColor(1.0,0.0, 0.0, 0.0);
				glClear(GL_COLOR_BUFFER_BIT);
 
 /*
				glLoadIdentity();
				glColor3f(0.5, 1.0, 0.0); 
				glRotatef(rotation, 0.0, 0.0, 1.0);
				glBegin(GL_TRIANGLES);
				//glBegin(GL_LINES);
					glVertex3f(-1, -.5, 0);
					glVertex3f(0, 1, 0);
					glVertex3f(1, 0, 0);
				glEnd();
*/
				//render();
				draw();

				context->SwapBuffer();
			}



			std::chrono::duration<double> calculationTime = std::chrono::high_resolution_clock::now() - frameStart;
			std::chrono::duration<double> diffToFrameEnd = m_frameDelay - calculationTime;


			log::debug << "frameDeltaTime: "  << (float)frameDeltaTime.count() << "s, Calculation time: " << (float)calculationTime.count() << "s" << std::endl;

			std::this_thread::sleep_for(diffToFrameEnd);

			frameEnd = std::chrono::high_resolution_clock::now();
		}
		
	}
	

}