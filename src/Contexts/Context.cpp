
#include <thread>

#include <xf86drm.h>
#include <xf86drmMode.h>
#include <gbm.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>


#include "Context.hpp"

#include <Systems.hpp>

#ifdef IS_EDITOR
#include "ImguiXlib.hpp"
#endif


namespace asapgl
{
	/*
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
        EGL_NONE};*/


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



	#ifdef USE_XLIB
	void Context::initXlib(const int argc, const char** argv)
	{
    	bfu::CallbackId id;
		if(context==0)
		{
			bfu::EventSystem& events = SYSTEMS::GetObject().EVENTS;

			//context = new Xlib_EGL_ContextType( attributes, contextAttribs, argc, argv );			
		}
		else
		{
			log::warning << "Context already initialized, skipping" << std::endl;
		}
	}
	#endif

	void Context::initDRM(const int argc, const char** argv)
	{
    	bfu::CallbackId id;
		if(context==0)
		{
			bfu::EventSystem& events = SYSTEMS::GetObject().EVENTS;

			//context = new DRM_GBM_EGL_ContextType( attributes, contextAttribs, argc, argv );
		}
		else
		{
			log::warning << "Context already initialized, skipping" << std::endl;
		}
	}



	void Context::MainLoop()
	{
		
		// bfu::EventSystem& events = SYSTEMS::GetObject().EVENTS;
		// RendererSystem& rendererSystem = SYSTEMS::GetObject().RENDERER;
		// auto frameEnd =  std::chrono::system_clock::now();
		// auto frameStart = std::chrono::high_resolution_clock::now();
		// bool show_demo_window = true;

		// std::chrono::duration<double> elapsed;

		// GLfloat rotation = 0.0;
		// while(m_isRunning)
		// {
		// 	std::chrono::duration<double> frameDeltaTime = frameEnd - frameStart;
		// 	frameStart = std::chrono::high_resolution_clock::now();

		// 	context->HandleContextEvents();

		// 	//TODO frame stuff
		// 	{
		// 		rotation += frameDeltaTime.count();


		// 		#ifdef IS_EDITOR
				
		//         // // Start the Dear ImGui frame
		//         // ImGui_ImplOpenGL2_NewFrame();
		//         // ImGui_ImplXlib_NewFrame();
		//         // ImGui::NewFrame();

		//         // // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		//         // if (show_demo_window)
	 //         //    	ImGui::ShowDemoWindow(&show_demo_window);

		// 		context->RenderImGui();
		// 		#endif

		// 		rendererSystem.Render();
				
		// 		context->SwapBuffer();
		// 	}



		// 	std::chrono::duration<double> calculationTime = std::chrono::high_resolution_clock::now() - frameStart;
		// 	std::chrono::duration<double> diffToFrameEnd = m_frameDelay - calculationTime;


		// 	//log::debug << "frameDeltaTime: "  << (float)frameDeltaTime.count() << "s, Calculation time: " << (float)calculationTime.count() << "s" << std::endl;

		// 	std::this_thread::sleep_for(diffToFrameEnd);

		// 	frameEnd = std::chrono::high_resolution_clock::now();
		// }
		
	}
	

}