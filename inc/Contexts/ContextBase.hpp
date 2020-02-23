#ifndef _H_CONTEXTBASE
#define _H_CONTEXTBASE
#include "OpenGLWrapper.hpp"
#include "object.hpp"

namespace asapgl
{

	class ContextBase: public object
	{

	public:
		ContextBase(){};
		~ContextBase();

		virtual void DrawBuffer() = 0;

	};
}

#endif
