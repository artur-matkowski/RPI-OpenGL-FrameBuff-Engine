#include "Context.hpp"
#include "Systems.hpp"
#include <cstdio>
#include <cstdlib>
#include <csignal>

asapgl::Context c;

void signalHandler( int signum )
{
	c.CleanUp();
	exit(0);
}

using namespace asapgl;



int main(const int argc, const char** argv)
{

	signal(SIGINT, signalHandler);


#ifndef _TARGET
	char* display = getenv("DISPLAY");

	bool hasDisplay = display != 0;


	if(hasDisplay)
	{
		//log::info << "DISPLAY=" << display << " Initializing GLUT context" << std::endl;

	    c.initXlib( argc, argv );
	}
	else
#endif
	{
		//log::info << "No DISPLAY found. Initializing DRM GBM EGL context" << std::endl;

		c.initDRM( argc, argv );
	}




    c.MainLoop();
    return 0;
}