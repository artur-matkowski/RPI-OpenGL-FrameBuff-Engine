#include "Context.hpp"
#include "Systems.hpp"
#include <SerializeJSON.hpp>
#include "Tests.hpp"

asapgl::Context c;

void signalHandler( int signum )
{
	c.CleanUp();
}

using namespace asapgl;



int main(int argc, char** argv)
{

	signal(SIGINT, signalHandler);
/*
	char* display = getenv("DISPLAY");

	bool hasDisplay = display != 0;


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
	else
	{
		Debug::Trace(DebugLevel::INFO) << "No DISPLAY found. Initializing DRM GBM EGL context" << std::endl;

		c.init<asapgl::DRM_GBM_EGL_ContextType>([&] ( asapgl::DRM_GBM_EGL_ContextType::Args& x ) {
			
		});
	}




    c.MainLoop();
*/


	TESTSerializableVar(bool, false);
	TESTSerializableVar(bool, true);

	TESTSerializableVar(float, 2);
	TESTSerializableVar(float, -2.342);


	TESTSerializableVar(int, 2);
	TESTSerializableVar(int, -20);

	TESTSerializableVar(std::string, "gowno test 213.ad,das");


	

	TESTSerializableVarVector(bool, true, false, true, true); 

	TESTSerializableVarVector(float, 1.2, 2.3, 3.4, 4,5 ); 

	TESTSerializableVarVector(int, 1, 2, 3, 4, 7 ); 

	TESTSerializableVarVector(std::string, "test 1", "test 2", "test 3", "test 4", "test 5" ); 


	{
		testClass obj;


		Debug::Trace(DebugLevel::INFO) << "testing  obj  " << obj.Serialize() << std::endl;
	}


    return 0;
}