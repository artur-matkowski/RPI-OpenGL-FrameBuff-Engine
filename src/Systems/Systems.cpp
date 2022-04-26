#include "Systems.hpp"
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>

#define HELP \
"Parameters accepted by this executable:"\
"\n\t?              <- prints this help message"\
"\n\t-help          <- prints this help message"\
"\n\t-compile (...) <- [editor-only] refresh resources on start, and close immediately"\
"\n\t-path (...)    <- select path for project to read (default path is: './'"



namespace asapi
{
	SYSTEMS SYSTEMS::_this;

	SYSTEMS::SYSTEMS()
		:MEMORY()
		,SCENE()
	{

	}


	static ContextBase* ContextInit()
	{
		static ContextBase* ret = 0;

		if(ret!=0)
			return ret;

		#ifndef IS_TARGET
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
		#endif
		{
		    ret = SYSTEMS::ALLOCATE<DRM_GBM_EGL_ContextType>(1);
		    new (ret) DRM_GBM_EGL_ContextType();
		}

		return ret;
	}


	bool SYSTEMS::init(const int argc, const char** argv)
	{
		if( argc>1 && ((argv[1][0]=='?') || (strcmp(argv[1], "-help")==0)) ){
			log::info << HELP << std::endl;
			exit(0);
		}

		const char* projectPath = 0;

		for(int i=1; i<argc; i++)
		{
			if( strcmp(argv[i], "-path")==0 )
			{
				projectPath = argv[i+1];
			}
		}

		//srand (time(NULL)); // moved to TIME.RANDOM

		PRIFILE( CONTEXT = ContextInit(); );
		PRIFILE( CONTEXT->Init(argc, argv); );
		PRIFILE( RENDERER.Init(); );

		PRIFILE( SCENE.Init( SYSTEMS::SYSTEMS_ALLOCATOR, argc, argv ); );

		
		RESOURCES.Init();
		if( projectPath!=0 )
		{
			SetProjectPath( projectPath );
		}


		#ifdef IS_EDITOR
			STUDIO.Init(argc, argv);
			log::info << "GL initialized with version: " << glGetString(GL_VERSION) << std::endl;
			log::info << "GL vendor: " << glGetString(GL_VENDOR) << std::endl;
			log::info << "GL renderer: " << glGetString(GL_RENDERER) << std::endl;
			log::info << "GL shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
		#endif


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
	void SYSTEMS::SystemsUpdate()
	{
		RESOURCES.Update();
	}
	
	void SYSTEMS::SetProjectPath(const char* path)
	{
		IsCorrectProjectPathSet = true;
		//TODO check if project path is actually correct

		
		RESOURCES.SetProjectPath( path );
		MATERIALSSYSTEM.SetProjectPath( path );

		PrefabLoaderComponent::s_projectPath = path;
	}

	void SYSTEMS::RefreshResources()
	{
		if( ! IsCorrectProjectPathSet )
			return;
		
		
		RESOURCES.RefreshResources();
		MATERIALSSYSTEM.RefreshResources();
	}
}