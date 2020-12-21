#ifndef _H_SYSTEMS
#define _H_SYSTEMS
#include "Singleton.hpp"
#include "object.hpp"
#include <bitforge/utils/bfu.hpp>
#include "RendererSystem.hpp"
#include "Context.hpp"
#include "ResourceSystem.hpp"

namespace asapgl
{
	class SYSTEMS: public object, public Singleton<SYSTEMS>
	{
	public:
		bfu::EventSystem 	EVENTS;
		RendererSystem 		RENDERER;
		Context  			CONTEXT;
		ResourceSystem		RESOURCES;


		bool init(const int argc, const char** argv);
		void cloaseApp();
		void mainAppLoop();
	
	};

}

#endif