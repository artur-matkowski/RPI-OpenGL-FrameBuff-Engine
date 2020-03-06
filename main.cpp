#include "Context.hpp"
#include "Systems.hpp"
#include <SerializableVarVector.hpp>
#include "Tests.hpp"

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

/*
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
*/
	bool test = true;
	
	test = test &= TESTJSONStream(float, -2.342);
	test = test &= TESTJSONStream(float, 2);


	test = test &= TESTJSONStream(int, 3);
	test = test &= TESTJSONStream(int, -2);


	test = test &= TESTJSONStream(bool, false);
	test = test &= TESTJSONStream(bool, true);


	test = test &= TESTJSONStream(std::string, "gowno test \\\"213.ad,das");



	test = test &= TESTJSONStreamVector(bool, true, false, true, true); 

	test = test &= TESTJSONStreamVector(float, 1.2, -2.3, 3.4, -4.5 ); 

	test = test &= TESTJSONStreamVector(int, 1, -2, 3, -4, 7 ); 

	test = test &= TESTJSONStreamVector(std::string, "test 1", "test 2", "test 3", "test 4", "test 5" ); 



	if( test )
	{
		Debug::Trace(DebugLevel::WARNING) << "<<<<<<<<<<<<<<<< TESTS concluded : SUCCES\n" << std::endl;
	}
	else
	{
		Debug::Trace(DebugLevel::ERROR) << "<<<<<<<<<<<<<<<< TESTS concluded : FAILED\n" << std::endl;	
	}

	
	{
		testClass obj;
		JSONStream json;

		json << obj;
		json.SetCursonPos(0);


		Debug::Trace(DebugLevel::INFO) << "testing  obj  " << json.str() << std::endl;
	}


    return 0;
}