#include "Context.hpp"
#include <iostream>
#include <csignal>

#include <chrono>
#include <thread>
#include <cstdlib>

asapgl::Context c;

void signalHandler( int signum )
{
	c.CleanUp();
}

int main(int argc, char** argv)
{

	signal(SIGINT, signalHandler);

	char* display = getenv("DISPLAY");

	bool hasDisplay = display != 0;

/*
	if(hasDisplay)
	{
		Debug::Trace(DebugLevel::INFO) << "DISPLAY=" << display << " Initializing GLUT context" << std::endl;

	    c.init<asapgl::GLUT_ContextType>( [&] (asapgl::GLUT_ContextType::Args &x) {  
	    	x.WIDTH = 1024;
	    	x.HEIGHT = 680;
	    	x.ARGC = argc;
	    	x.ARGV = argv;
	    	x.NAME = "Window name";
	    	x.DISPLAY_MODE = GLUT_DOUBLE | GLUT_RGB;
	    });
	}
	else*/
	{
		Debug::Trace(DebugLevel::INFO) << "No DISPLAY found. Initializing DRM GBM EGL context" << std::endl;

		c.init<asapgl::DRM_GBM_EGL_ContextType>([&] ( asapgl::DRM_GBM_EGL_ContextType::Args& x ) {
			
		});
	}




    c.MainLoop();




    return 0;
}