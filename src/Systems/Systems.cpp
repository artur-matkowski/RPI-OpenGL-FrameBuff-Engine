#include "Systems.hpp"
#include <stdlib.h>
#include <time.h>


namespace asapgl
{
	bool SYSTEMS::init(const int argc, const char** argv)
	{
		srand (time(NULL));
		RENDERER.SetupEvents();

	#ifndef IS_PLAYER
		char* display = getenv("DISPLAY");

		bool hasDisplay = display != 0;


		if(hasDisplay)
		{
			//log::info << "DISPLAY=" << display << " Initializing GLUT context" << std::endl;

		    CONTEXT.initXlib( argc, argv );
		}
		else
	#endif
		{
			//log::info << "No DISPLAY found. Initializing DRM GBM EGL context" << std::endl;

			CONTEXT.initDRM( argc, argv );
		}

		RENDERER.Init();


		return true;

	}
	
	void SYSTEMS::cloaseApp()
	{
		CONTEXT.CleanUp();
	}

	void SYSTEMS::mainAppLoop()
	{
    	CONTEXT.MainLoop();
	}
}