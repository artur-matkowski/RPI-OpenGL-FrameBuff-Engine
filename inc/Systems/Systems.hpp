#ifndef _H_SYSTEMS
#define _H_SYSTEMS
#include "object.hpp"
#include <bitforge/utils/bfu.hpp>
#include "RendererSystem.hpp"
#include "Context.hpp"
#include "ResourceSystem.hpp"
#include "TimeSystem.hpp"
#include "MemoryManagmentSystem.hpp"
#include "SceneSystem.hpp"
#include "EditorSystem.hpp"
#include <stdlib.h>

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

		static SYSTEMS& GetObject()
		{
			return _this;
		}


		
		#ifdef IS_EDITOR
		void OnGUI();
		#endif

	
	};

}

#endif