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
12

	TESTSerializableVar(int, 2);
	TESTSerializableVar(int, -20);

	TESTSerializableVar(std::string, "gowno test 213.ad,das");


	

	TESTSerializableVarVector(bool, true, false, true, true); 

	TESTSerializableVarVector(float, 1.2, 2.3, 3.4, 4,5 ); 

	TESTSerializableVarVector(int, 1, 2, 3, 4, 7 ); 

	TESTSerializableVarVector(std::string, "test 1", "test 2", "test 3", "test 4", "test 5" ); 
*/
	bool test = true;
	
	test = test && TESTJSONStream(float, -2.342);
	test = test && TESTJSONStream(float, 2);


	test = test && TESTJSONStream(int, 3);
	test = test && TESTJSONStream(int, -2);


	test = test && TESTJSONStream(bool, false);
	test = test && TESTJSONStream(bool, true);


	test = test && TESTJSONStream(std::string, "gowno test \\\"213.ad,das");



	test = test && TESTJSONStreamVector(bool, true, false, true, true); 

	test = test && TESTJSONStreamVector(float, 1.2, -2.3, 3.4, -4.5 ); 

	test = test && TESTJSONStreamVector(int, 1, -2, 3, -4, 7 ); 

	test = test && TESTJSONStreamVector(std::string, "test 1", "test 2", "test 3", "test 4", "test 5" ); 




	{
		JSONStream json;
		JSONStream json2;
		testClass tt;
		testClass tt2;
		//tt = val;

		json << tt;

		json.SetCursonPos(0);

		json >> tt2;
		json2 << tt2;

		Debug::Trace(DebugLevel::INFO) << "Testing: testClass2" 
				//<< "\n\tOriginal input:\n\t\t>" << tt 
		 		<< "<\n\tSerialized to JSON:\n\t\t>" << json.str()  
		 		//<< "<\n\tDeserialized back to type:\n\t\t>" << tt2
		 		<< "<\n\tSerialized to JSON2:\n\t\t>" << json2.str()  
				<< "<\n" << std::endl;

		if( std::strcmp(json.str().c_str(), json2.str().c_str() )==0 )
		{
			Debug::Trace(DebugLevel::WARNING) << "<<<<<<<<<<<<<<<< Test concluded : SUCCES\n" << std::endl;
			test = test && true;
		}
		else
		{
			Debug::Trace(DebugLevel::ERROR) << "<<<<<<<<<<<<<<<< Test concluded : FAILED\n" << std::endl;
			test = test && false;		
		}
	}




	/*
	{
		testClass obj;
		testClass obj2;
		JSONStream json;
		JSONStream json2;

		json << obj;
		json.SetCursonPos(0);


		Debug::Trace(DebugLevel::INFO) << "testing  obj  " << json.str() << std::endl;
	}
*/


	if( test )
	{
		Debug::Trace(DebugLevel::WARNING) << "<<<<<<<<<<<<<<<< TESTS concluded : SUCCES\n" << std::endl;
	}
	else
	{
		Debug::Trace(DebugLevel::ERROR) << "<<<<<<<<<<<<<<<< TESTS concluded : FAILED\n" << std::endl;	
	}


    return 0;
}