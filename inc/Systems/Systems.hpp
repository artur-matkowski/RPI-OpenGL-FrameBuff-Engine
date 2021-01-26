#ifndef _H_SYSTEMS
#define _H_SYSTEMS
#include "Singleton.hpp"
#include "object.hpp"
#include <bitforge/utils/bfu.hpp>
#include "RendererSystem.hpp"
#include "Context.hpp"
#include "ResourceSystem.hpp"
#include "SimpleTime.hpp"
#include "MemoryManagmentSystem.hpp"

namespace asapgl
{
	class SYSTEMS: public object, public Singleton<SYSTEMS>
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

		bool init(const int argc, const char** argv);
		void cloaseApp();
		void mainAppLoop();

		
		void OnGUI();
	
	};

}

#endif