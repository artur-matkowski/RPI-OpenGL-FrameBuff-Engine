#include "Systems.hpp"
#include <stdlib.h>
#include <time.h>
#include "ImguiXlib.hpp"


namespace asapgl
{
	#ifdef USE_XLIB
	static ContextBase* ContextInit()
	{
		static ContextBase* ret = 0;

		if(ret!=0)
			return ret;

		char* display = getenv("DISPLAY");

		bool hasDisplay = display != 0;


		if(hasDisplay)
		{
		    ret = new Xlib_EGL_ContextType();
		}
		else
		{
		    ret = new DRM_GBM_EGL_ContextType();
		}

		return ret;
	}
	#endif


	bool SYSTEMS::init(const int argc, const char** argv)
	{
		srand (time(NULL));

		RENDERER.SetupEvents();
		#ifdef USE_XLIB
		CONTEXT = ContextInit();
		CONTEXT->Init(argc, argv);
		#else
		CONTEXT.Init(argc, argv);
		#endif
		RENDERER.Init();

		log::info << "GL initialized with version: " << glGetString(GL_VERSION) << std::endl;
		log::info << "GL vendor: " << glGetString(GL_VENDOR) << std::endl;
		log::info << "GL renderer: " << glGetString(GL_RENDERER) << std::endl;
		log::info << "GL shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


		return true;

	}
	
	void SYSTEMS::cloaseApp()
	{
		#ifdef USE_XLIB
		CONTEXT->CleanUp();
		#else
		CONTEXT.CleanUp();
		#endif
	}

	void SYSTEMS::mainAppLoop()
	{
		#ifdef USE_XLIB
		CONTEXT->MainLoop();
		#else
		CONTEXT.MainLoop();
		#endif
	}

	void SYSTEMS::OnGUI()
	{


		// glViewport(0, 0, m_mainEglWindow->resolution.x, m_mainEglWindow->resolution.y);
		// glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// glClear(GL_COLOR_BUFFER_BIT);
		
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplXlib_NewFrame();
        ImGui::NewFrame();

    
        ImGui::ShowDemoWindow(0);

	            


	    MEMORY.OnGUI();
	}
}