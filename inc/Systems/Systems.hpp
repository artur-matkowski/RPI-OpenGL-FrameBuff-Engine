#ifndef _H_SYSTEMS
#define _H_SYSTEMS
#include "object.hpp"
#include <bitforge/utils/bfu.hpp>
#include "RendererSystem.hpp"
#include "Context.hpp"
#include "ResourceSystem.hpp"
#include "SimpleTime.hpp"
#include "MemoryManagmentSystem.hpp"
#include "SceneSystem.hpp"

namespace asapgl
{
	class SYSTEMS: public object
	{
	public:
		MemoryManagmentSystem 	MEMORY;

		bfu::EventSystem 		EVENTS;
		RendererSystem 			RENDERER;
		#ifdef USE_XLIB
		ContextBase				*CONTEXT;
		#else
		DRM_GBM_EGL_ContextType			
								CONTEXT;
		#endif
		ResourceSystem			RESOURCES;
		SimpleTime				TIME;
		SceneSystem				SCENE;

		bool init(const int argc, const char** argv);
		void cloaseApp();
		void mainAppLoop();

		static SYSTEMS& GetObject()
		{
			static SYSTEMS _this;
			return _this;
		}

		SYSTEMS();

		
		void OnGUI();


	
	};

}

#endif