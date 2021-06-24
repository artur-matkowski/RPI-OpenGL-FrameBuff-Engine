#ifndef _H_SYSTEMS
#define _H_SYSTEMS
#include <stdlib.h>
#include <sys/stat.h>
#include "bfu.hpp"

#include "object.hpp"
#include "RendererSystem.hpp"
#include "Context.hpp"
#include "ResourceSystem.hpp"
#include "TimeSystem.hpp"
#include "MemoryManagmentSystem.hpp"
#include "SceneSystem.hpp"
#ifdef IS_EDITOR
#include "StudioSystem.hpp"
#endif

namespace asapi
{
	class SYSTEMS: public object
	{
		static SYSTEMS _this;
		SYSTEMS();
	public:
		MemoryManagmentSystem 	MEMORY;

		bfu::EventSystem 		EVENTS;
		RendererSystem 			RENDERER;
		ContextBase				*CONTEXT;
		ResourceSystem			RESOURCES;
		TimeSystem				TIME;
		SceneSystem				SCENE;


		#ifdef IS_EDITOR
		StudioSystem			STUDIO;
		#endif

		bool init(const int argc, const char** argv);
		void cloaseApp();
		void mainAppLoop();

		inline static SYSTEMS& GetObject()
		{
			return _this;
		}


		

		struct IO
		{
			class MMAP
			{
				void* data = nullptr;
				int fd = -1;
				struct stat sb;

			public:
				MMAP(){};
				~MMAP();

				void InitForRead(const char* filename);
				void InitForWrite(const char* filename, size_t size);

				void Close();

				bool IsValid(){ return fd!=-1; }
				void* Data(){ return data; }
				uint32_t Size(){ return sb.st_size; }

			};

			class STREAM
			{
				int fd = -1;

			public:
				STREAM(){};
				~STREAM();

				void Close();

				void InitForWrite(const char* filename);
				void Write(const char* buff, const int size);
			};
		};
	};

}

#endif