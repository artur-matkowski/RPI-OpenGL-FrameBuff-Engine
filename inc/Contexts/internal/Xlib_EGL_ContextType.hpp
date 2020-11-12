#ifndef _H_Xlib_EGL_ContextType
#define _H_Xlib_EGL_ContextType
#include "ContextBase.hpp"

namespace asapgl
{


	class Xlib_EGL_ContextType: public ContextBase
	{
		struct XlibData;	
		struct XlibData *m_XlibData;

		struct EGLData;	
		struct EGLData *m_eglData;

		bool DisplayOpen(void);
		bool window_create(const char *name,
				    unsigned int x, unsigned int y,
				    unsigned int width, unsigned int height,
				    const int* attributes, const int* contextAttribs);
		void window_show();

	public:

		Xlib_EGL_ContextType(const int* attributes, const int* contextAttribs, const int argc, const char** argv);
		~Xlib_EGL_ContextType();

		void SwapBuffer();
		void GetResolution(int & width, int & height);
	};

}

#endif