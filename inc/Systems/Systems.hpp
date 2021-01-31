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
#include "EditorSystem.hpp"
#include <stdlib.h>

namespace asapgl
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
		DRM_GBM_EGL_ContextType			
								CONTEXT;
		#endif
		ResourceSystem			RESOURCES;
		SimpleTime				TIME;
		SceneSystem				SCENE;


		#ifdef IS_EDITOR
		EditorSystem			EDITOR;
		#endif

		bool init(const int argc, const char** argv);
		void cloaseApp();
		void mainAppLoop();

		static SYSTEMS& GetObject()
		{
			// static SYSTEMS* _this = 0;
			// if(_this==0)
			// {
			// 	_this = (SYSTEMS*)aligned_alloc(alignof(SYSTEMS), sizeof(SYSTEMS));
			// 	new (_this) SYSTEMS();
			// }
			return _this;
		}


		
		#ifdef IS_EDITOR
		void OnGUI();
		#endif

	
	};

}

#endif