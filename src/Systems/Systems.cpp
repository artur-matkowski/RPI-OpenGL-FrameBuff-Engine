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
		//srand (time(NULL)); // moved to TIME.RANDOM

		PRIFILE( CONTEXT = ContextInit(); );
		PRIFILE( CONTEXT->Init(argc, argv); );
		PRIFILE( RENDERER.Init(); );

		PRIFILE( SCENE.Init( SYSTEMS::SYSTEMS_ALLOCATOR, argc, argv ); );

		RESOURCES.Init(argc, argv);
		ASSETS.Init(argc, argv);

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

		if( sb.st_size==0 )
		{
			log::error << "File size is 0: " << filename << std::endl;
			return;
		}

		data = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

		if (data == MAP_FAILED)
			log::error << "Failed to mmap file: " << filename << std::endl;		
	}

	void SYSTEMS::IO::MMAP::InitForWrite(const char* filename, size_t size)
	{
		unlink(filename);
		fd = open(filename, O_RDWR | O_CREAT | O_TRUNC | O_SYNC, (mode_t)0666);
		if (fd == -1)
		{
			log::error << "Can not open file: " << filename << std::endl;
			return;
		}

		const int pageSize = getpagesize();
		sb.st_size = size / pageSize + pageSize;

		data = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		ftruncate(fd, sb.st_size);
		bzero(data, sb.st_size);

		if (data == MAP_FAILED)
			log::error << "Failed to mmap file: " << filename << std::endl;	
	}

	SYSTEMS::IO::MMAP::~MMAP()
	{
		if(data != MAP_FAILED && data != nullptr)
		{
			msync(data, sb.st_size, MS_SYNC);
			munmap(data, sb.st_size);
		}
		if(fd!=-1)
			close(fd);
	}

	void SYSTEMS::IO::MMAP::Close()
	{
		if(data != MAP_FAILED && data != nullptr)
		{
			msync(data, sb.st_size, MS_SYNC);
			munmap(data, sb.st_size);
		}
		if(fd!=-1)
			close(fd);

		data = nullptr;
		fd = -1;
	}


	SYSTEMS::IO::STREAM::~STREAM()
	{
		Close();
	}

	void SYSTEMS::IO::STREAM::Close()
	{
		if(fd!=-1)
			close(fd);
		
		fd = -1;
	}


	void SYSTEMS::IO::STREAM::InitForWrite(const char* filename)
	{
		unlink(filename);
		fd = open(filename, O_RDWR | O_CREAT | O_TRUNC | O_SYNC, (mode_t)0666);
		if (fd == -1)
		{
			log::error << "Can not open file: " << filename << std::endl;
			return;
		}
	}


	void SYSTEMS::IO::STREAM::Write(const char* buff, const int size)
	{
		write(fd, buff, size);
	}
}