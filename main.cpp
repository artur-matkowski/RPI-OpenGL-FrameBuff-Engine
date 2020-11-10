#include "Context.hpp"
#include "Systems.hpp"

asapgl::Context c;

void signalHandler( int signum )
{
	c.CleanUp();
	exit(0);
}

using namespace asapgl;



int main(int argc, char** argv)
{

	signal(SIGINT, signalHandler);

	char* display = getenv("DISPLAY");

	bool hasDisplay = display != 0;


	if(hasDisplay)
	{
		//log::info << "DISPLAY=" << display << " Initializing GLUT context" << std::endl;

	    c.init<asapgl::Xlib_EGL_ContextType>( [&] (asapgl::Xlib_EGL_ContextType::Args &x) {  
	    	/*
	    	x.WIDTH = 1024;
	    	x.HEIGHT = 680;
	    	x.ARGC = argc;
	    	x.ARGV = argv;
	    	x.NAME = "Window name";
	    	x.DISPLAY_MODE = GLUT_DOUBLE | GLUT_RGB;*/
	    });
	}
	else
	{
		//log::info << "No DISPLAY found. Initializing DRM GBM EGL context" << std::endl;

		c.init<asapgl::DRM_GBM_EGL_ContextType>([&] ( asapgl::DRM_GBM_EGL_ContextType::Args& x ) {
			
		});
	}




    c.MainLoop();
    return 0;
}