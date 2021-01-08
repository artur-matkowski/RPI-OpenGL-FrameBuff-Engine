#include "Systems.hpp"
#include <stdlib.h>
#include <time.h>


namespace asapgl
{
	static ContextBase* ContextInit()
	{
		static ContextBase* ret = 0;

		if(ret!=0)
			return ret;

		#ifdef USE_XLIB
		char* display = getenv("DISPLAY");

		bool hasDisplay = display != 0;


		if(hasDisplay)
		{
		    ret = new Xlib_EGL_ContextType();
		}
		else
		#endif
		{
		    ret = new DRM_GBM_EGL_ContextType();
		}

		return ret;
	}


	bool SYSTEMS::init(const int argc, const char** argv)
	{
		srand (time(NULL));

		RENDERER.SetupEvents();
		CONTEXT = ContextInit();
		CONTEXT->Init(argc, argv);
		RENDERER.Init();

		log::info << "GL initialized with version: " << glGetString(GL_VERSION) << std::endl;
		log::info << "GL vendor: " << glGetString(GL_VENDOR) << std::endl;
		log::info << "GL renderer: " << glGetString(GL_RENDERER) << std::endl;
		log::info << "GL shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


		return true;

	}
	
	void SYSTEMS::cloaseApp()
	{
		CONTEXT->CleanUp();
	}

	void SYSTEMS::mainAppLoop()
	{
    	CONTEXT->MainLoop();
	}
}