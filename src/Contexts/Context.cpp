#include "Context.hpp"
#include <chrono>
#include <thread>
#include <iostream>

namespace asapgl
{
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
			//m_isRunning = false;
			std::chrono::duration<double> frameDeltaTime = frameEnd - frameStart;
			frameStart = std::chrono::high_resolution_clock::now();

			

			//TODO frame stuff
			{
				rotation += frameDeltaTime.count();
				glClearColor(1.0,0.0, 0.0, 0.0);
				glClear(GL_COLOR_BUFFER_BIT);
 
				glLoadIdentity();
				glColor3f(0.5, 1.0, 0.0); 
				glRotatef(rotation, 0.0, 0.0, 1.0);
				glBegin(GL_TRIANGLES);
				//glBegin(GL_LINES);
					glVertex3f(-1, -.5, 0);
					glVertex3f(0, 1, 0);
					glVertex3f(1, 0, 0);
				glEnd();
				

				context->SwapBuffer();
			}



			std::chrono::duration<double> calculationTime = std::chrono::high_resolution_clock::now() - frameStart;
			std::chrono::duration<double> diffToFrameEnd = m_frameDelay - calculationTime;


			Debug::Trace << "frameDeltaTime: "  << frameDeltaTime.count() << "s, Calculation time: " << calculationTime.count() << "s" << std::endl;

			std::this_thread::sleep_for(diffToFrameEnd);

			frameEnd = std::chrono::high_resolution_clock::now();
		}
		
	}
	

}