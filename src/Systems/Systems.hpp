#ifndef _H_SYSTEMS
#define _H_SYSTEMS
#include <stdlib.h>
#include <sys/stat.h>
#include <bitforge/utils/bfu.hpp>

#include "object.hpp"
#include "RendererSystem.hpp"
#include "Context.hpp"
#include "ResourceSystem.hpp"
#include "TimeSystem.hpp"
#include "MemoryManagmentSystem.hpp"
#include "SceneSystem.hpp"
#include "EditorSystem.hpp"

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
		#ifdef USE_XLIB
		ContextBase				*CONTEXT;
		#else
		DRM_GBM_EGL_ContextType	CONTEXT;
		#endif
		ResourceSystem			RESOURCES;
		TimeSystem				TIME;
		SceneSystem				SCENE;


		#ifdef IS_EDITOR
		EditorSystem			EDITOR;
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
			static void OpenFile(FILE** ret_pFile, long int* ret_filesize, const char* filename);
			static inline void ReadFile(FILE* pFile, char* buff, uint32_t buffsize)
			{ 
				fread(buff, sizeof(char), buffsize, pFile); 
			}
			static inline void WriteFile(FILE* pFile, char* buff, uint32_t buffsize)
			{
				fwrite(buff, 1, buffsize, pFile);
			}
			static inline void CloseFile(FILE* pFile)
			{ 
				fclose (pFile); 
			}

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

				bool IsValid(){ return fd!=-1; }
				void* Data(){ return data; }
				uint32_t Size(){ return sb.st_size; }

			};
		};
	};

}

#endif