#include "Systems.hpp"
#include <stdlib.h>
#include <time.h>

#define HELP \
"Parameters accepted by this executable:"\
"\n\t?              <- prints this help message"\
"\n\t-json          <- [player-only] start player using json files instaed of mmp"\
"\n\t-path (...)    <- select path for project to read (default path is: './'"



namespace asapi
{
	SYSTEMS SYSTEMS::_this;

	SYSTEMS::SYSTEMS()
		:MEMORY()
		,SCENE()
	{

	}


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
		    // ret = SYSTEMS::ALLOCATE<Xlib_EGL_ContextType>(1);
		    // new (ret) Xlib_EGL_ContextType();
		    ret = SYSTEMS::ALLOCATE<GLFW_egl_Context>(1);
		    new (ret) GLFW_egl_Context();
		}
		else
		{
		    ret = SYSTEMS::ALLOCATE<DRM_GBM_EGL_ContextType>(1);
		    new (ret) DRM_GBM_EGL_ContextType();
		}

		return ret;
	}
	#endif


	bool SYSTEMS::init(const int argc, const char** argv)
	{
		if( argc>1 && argv[1][0]=='?'){
			log::info << HELP << std::endl;
			exit(0);
		}
		//srand (time(NULL)); // moved to TIME.RANDOM

		#ifdef USE_XLIB
		PRIFILE( CONTEXT = ContextInit(); );
		PRIFILE( CONTEXT->Init(argc, argv); );
		#else
		PRIFILE( CONTEXT.Init(argc, argv); );
		#endif
		PRIFILE( RENDERER.Init(); );

		PRIFILE( SCENE.Init( SYSTEMS::SYSTEMS_ALLOCATOR, argc, argv ); );

		#ifdef IS_EDITOR
			EDITOR.Init();
			log::info << "GL initialized with version: " << glGetString(GL_VERSION) << std::endl;
			log::info << "GL vendor: " << glGetString(GL_VENDOR) << std::endl;
			log::info << "GL renderer: " << glGetString(GL_RENDERER) << std::endl;
			log::info << "GL shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
		#endif


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


	void SYSTEMS::IO::OpenFile(FILE** ret_pFile, long int* ret_filesize, char* filename)
	{
		*ret_pFile = fopen (filename,"rb");

		if(*ret_pFile == 0)
		{
			*ret_filesize = -1;
			return;
		}

		fseek(*ret_pFile, 0L, SEEK_END); 
		*ret_filesize = ftell(*ret_pFile); 
		fseek(*ret_pFile, 0L, SEEK_SET);
	}
}