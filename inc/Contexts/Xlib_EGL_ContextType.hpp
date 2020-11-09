#ifndef _H_Xlib_EGL_ContextType
#define _H_Xlib_EGL_ContextType
#include "ContextBase.hpp"

namespace asapgl
{

	class Xlib_EGL_ContextType: public ContextBase
	{
	public:
		class Args: public object{
		public:
		};

		Xlib_EGL_ContextType(Xlib_EGL_ContextType::Args &f);
		~Xlib_EGL_ContextType();

		void SwapBuffer();
	};

}

#endif