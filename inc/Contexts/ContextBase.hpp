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

		virtual void SwapBuffer() = 0;

	};
}

#endif
