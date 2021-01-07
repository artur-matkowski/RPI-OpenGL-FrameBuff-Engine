#ifndef _H_Xlib_EGL_ContextType
#define _H_Xlib_EGL_ContextType
#ifndef IS_PLAYER
#include "ContextBase.hpp"
#include "KeyCodes.hpp"

#include <X11/Xlib.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>


namespace asapgl
{


	class Xlib_EGL_ContextType: public ContextBase
	{
		struct XDisplay
		{
			Display *display;
			EGLDisplay egl;
			Window root;
		};

		struct EGLWindow
		{
			EGLContext context;
			EGLSurface surface;
			unsigned int width;
			unsigned int height;


			long unsigned int x11;
		};

		int renderTgt = 0;


		struct XDisplay m_XDisplay;

		std::vector<struct EGLWindow> m_eglWindows;



		void InitMaps();

		bool DisplayOpen(void);
		bool window_create(const char *name,
				    unsigned int x, unsigned int y,
				    unsigned int width, unsigned int height,
				    const int* attributes, const int* contextAttribs,
				    EGLWindow& eglWindow);
		void window_show(EGLWindow& eglWindow);

	public:

		Xlib_EGL_ContextType(const int* attributes, const int* contextAttribs, const int argc, const char** argv);
		~Xlib_EGL_ContextType();

		virtual void SwapBuffer();
		virtual void HandleContextEvents();

		#ifndef IS_PLAYER
		virtual void RenderImGui() override;
		#endif
	};

}

#endif

#endif