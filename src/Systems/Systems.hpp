#ifndef _H_SYSTEMS
#define _H_SYSTEMS
#include <stdlib.h>
#include <sys/stat.h>
#include "bfu.hpp"

#include "object.hpp"
#include "RendererSystem.hpp"
#include "Context.hpp"

#include "ResourceSystem.hpp"
#include "_ResourceProcessorsInclude.hpp"

#include "ResourceTrackerManager.hpp"
#include "TimeSystem.hpp"
#include "MemoryManagmentSystem.hpp"
#include "SceneSystem.hpp"
#include "MaterialsSystem.hpp"
#ifdef IS_EDITOR
#include "StudioSystem.hpp"
#endif

namespace asapi
{
	class SYSTEMS: public object
	{
		static SYSTEMS _this;
		SYSTEMS();

		bool IsCorrectProjectPathSet = false;
	public:
		MemoryManagmentSystem 	MEMORY;

		bfu::EventSystem 		EVENTS;
		RendererSystem 			RENDERER;
		ContextBase				*CONTEXT;
		ResourceSystem<
			  ResourceTXTProcessor
			, ResourceFBXProcessor
			, ResourceGLSLProcessor
			, ResourcePNGProcessor
			>		 			RESOURCES;
		MaterialsSystem 		MATERIALSSYSTEM;
		TimeSystem				TIME;
		SceneSystem				SCENE;


		#ifdef IS_EDITOR
		StudioSystem			STUDIO;
		#endif

		bool init(const int argc, const char** argv);
		void SystemsUpdate();
		void cloaseApp();
		void mainAppLoop();

		inline static SYSTEMS& GetObject()
		{
			return _this;
		}

		void SetProjectPath(const char* path);
		void RefreshResources();
	};

}

#endif