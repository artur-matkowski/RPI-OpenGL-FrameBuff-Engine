#ifndef _H_RENDERERSYSTEM
#define _H_RENDERERSYSTEM
#include "object.hpp"

namespace asapgl{

	class RendererSystem: public object
	{
		int 				m_width;
		int 				m_height;

	public:
		RendererSystem();
		~RendererSystem();

		void SetupEvents();
		void Render();
	};

}


#endif