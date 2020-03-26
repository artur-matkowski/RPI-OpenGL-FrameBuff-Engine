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
				render();

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