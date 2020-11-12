#ifndef _H_CONTEXTBASE
#define _H_CONTEXTBASE
#include "object.hpp"

namespace asapgl
{

	class ContextBase: public object
	{

	public:
		ContextBase(){};
		~ContextBase();

		virtual void SwapBuffer() = 0;
		virtual void GetResolution(int & width, int & height) = 0;

	};
}

#endif
