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
			Display 	*display;
			EGLDisplay 	egl;
			Window 		root;
		};

		struct EGLWindow
		{
			EGLContext 	context;
			EGLSurface 	surface;
			glm::ivec2 	resolution;
			glm::ivec2 	position;
			glm::ivec2 	cursorPos;

			Window 		x11;
    		bool   		WindowOwned;
		};
	protected:

		int renderTgt = 0;


		struct XDisplay 				m_XDisplay;

		std::vector<struct EGLWindow> 	m_eglWindows;
		EGLWindow* 						m_mainEglWindow = 0;
		Window 							m_focusedWindow;




		#ifdef IS_EDITOR
		void RenderImGui();
		#endif


		void InitMaps();

		bool DisplayOpen(void);

	public:

		Xlib_EGL_ContextType()
			:ContextBase(){};
		~Xlib_EGL_ContextType();


		EGLWindow* window_create(const char *name,
				    unsigned int x, unsigned int y,
				    unsigned int width, unsigned int height);
		void window_show(EGLWindow& eglWindow);


		virtual void Init(const int argc, const char** argv) override;
		virtual void SwapBuffer() override;
		virtual void MainLoop() override;
		virtual void HandleContextEvents() override;
		virtual void CleanUp() override;

		inline Display* GetDisplay()
		{
			return m_XDisplay.display;
		}
		inline EGLDisplay GetEGLDisplay()
		{
			return m_XDisplay.egl;
		}
		inline Window GetRoot()
		{
			return m_XDisplay.root;
		}
		inline EGLWindow* GetMainEGLWindow()
		{
			return m_mainEglWindow;
		}
		inline Window GetFocusedWindow()
		{
			return m_focusedWindow;
		}
		inline void Erase(EGLWindow* data)
		{
			for(std::vector<struct EGLWindow>::iterator it = m_eglWindows.begin(); 
				it!=m_eglWindows.end(); 
				++it)
			{
				if( it->x11 == data->x11 )
				{
					m_eglWindows.erase(it);
					break;
				}
			}
		}

	};

}

#endif

#endif