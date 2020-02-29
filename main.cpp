#include "Context.hpp"
#include "Systems.hpp"
#include <SerializeJSON.hpp>

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
	SERIALIZABLE_VAR_VECTOR(float, var);
	SERIALIZABLE_VAR_VECTOR(float, var2);

	var.push_back(1.2);
	var.push_back(1.1);
	var.push_back(3.2);
	var.push_back(13.2);

	std::string json = var.Serialize();


	Debug::Trace(DebugLevel::INFO) << "testing  var  " << var.Serialize() << std::endl;


	var2.Deserialize(json.c_str(), json.size());

	Debug::Trace(DebugLevel::INFO) << "testing  var2  " << var2.Serialize() << std::endl;

    return 0;
}