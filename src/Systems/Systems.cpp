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
		if( argc>1 && argv[1][0]=='?'){
			log::info << HELP << std::endl;
			exit(0);
		}
		//srand (time(NULL)); // moved to TIME.RANDOM

		PRIFILE( CONTEXT = ContextInit(); );
		PRIFILE( CONTEXT->Init(argc, argv); );
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
		CONTEXT->CleanUp();
	}

	void SYSTEMS::mainAppLoop()
	{
		CONTEXT->MainLoop();
	}


	void SYSTEMS::IO::OpenFile(FILE** ret_pFile, long int* ret_filesize, const char* filename)
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


	void SYSTEMS::IO::MMAP::InitForRead(const char* filename)
	{
		fd = open(filename, O_RDONLY );
		if (fd == -1)
		{
			log::error << "Can not open file: " << filename << std::endl;
			return;
		}

		if (fstat(fd, &sb) == -1) 
		{
			log::error << "Can not fstat file: " << filename << std::endl;
			return;
		}

		data = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

		if (data == MAP_FAILED)
			log::error << "Failed to mmap file: " << filename << std::endl;		
	}

	void SYSTEMS::IO::MMAP::InitForWrite(const char* filename, size_t size)
	{
		fd = open(filename, O_RDWR | O_CREAT | O_TRUNC | O_SYNC, (mode_t)0666);
		if (fd == -1)
		{
			log::error << "Can not open file: " << filename << std::endl;
			return;
		}

		sb.st_size = size;

		data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);	
		ftruncate(fd, size);

		if (data == MAP_FAILED)
			log::error << "Failed to mmap file: " << filename << std::endl;	
	}

	SYSTEMS::IO::MMAP::~MMAP()
	{
		if(data != MAP_FAILED && data != nullptr)
			munmap(data, sb.st_size);
		if(fd!=-1)
			close(fd);
	}
}