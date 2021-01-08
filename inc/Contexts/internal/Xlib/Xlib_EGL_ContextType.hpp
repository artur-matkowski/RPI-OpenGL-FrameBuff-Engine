#ifndef _H_Xlib_EGL_ContextType
#define _H_Xlib_EGL_ContextType

#include "ContextBase.hpp"
#include "KeyCodes.hpp"

#include <X11/Xlib.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

#ifdef USE_XLIB

namespace asapgl
{


	class Xlib_EGL_ContextType: public ContextBase
	{
	public:
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
			glm::ivec2 resolution;

			Window x11;
		};
	protected:

		int renderTgt = 0;


		struct XDisplay 				m_XDisplay;

		std::vector<struct EGLWindow> 	m_eglWindows;
		EGLWindow* 						m_mainEglWindow = 0;



		void InitMaps();

		bool DisplayOpen(void);
		bool window_create(const char *name,
				    unsigned int x, unsigned int y,
				    unsigned int width, unsigned int height,
				    const int* attributes, const int* contextAttribs,
				    EGLWindow& eglWindow);
		void window_show(EGLWindow& eglWindow);

		#ifdef IS_EDITOR
		void RenderImGui();
		#endif
	public:

		Xlib_EGL_ContextType()
			:ContextBase(){};
		~Xlib_EGL_ContextType();

		virtual void Init(const int argc, const char** argv) override;
		virtual void SwapBuffer() override;
		virtual void MainLoop() override;
		virtual void HandleContextEvents() override;
		virtual void CleanUp() override;

	};

}

#endif

#endif