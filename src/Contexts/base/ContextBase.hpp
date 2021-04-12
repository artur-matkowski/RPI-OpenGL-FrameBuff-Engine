#ifndef _H_CONTEXTBASE
#define _H_CONTEXTBASE
#include <bitforge/utils/bfu.hpp>
#include <EGL/egl.h>
#include <chrono>
#ifdef USE_XLIB
#include <X11/Xlib.h>
#endif
#include "object.hpp"
#include "KeyCodes.hpp"

namespace asapi
{
	const EGLint attributes[] = {
	    EGL_BLUE_SIZE, 8, 
	    EGL_GREEN_SIZE, 8,
	    EGL_RED_SIZE, 8,

	    // Uncomment the following to enable MSAA
	    // EGL_SAMPLE_BUFFERS, 1, // <-- Must be set to 1 to enable multisampling!
	    // EGL_SAMPLES, 4, // <-- Number of samples

	    // Uncomment the following to enable stencil buffer
	    // EGL_STENCIL_SIZE, 1,

	    EGL_RENDERABLE_TYPE, 
	    EGL_OPENGL_ES2_BIT, 
	    //EGL_OPENGL_BIT,
	    EGL_NONE};
	    
	const EGLint contextAttribs[] = {
		EGL_CONTEXT_MAJOR_VERSION, 0,
		EGL_CONTEXT_MINOR_VERSION, 0,
		//EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE, EGL_TRUE,
        EGL_NONE};

	class ContextBase: public object
	{
	protected:
		bool				m_isRunning = true;
		std::chrono::duration<double> 
							m_frameDelay;

		bfu::Event* 		p_ev_ResizeWindow = nullptr;
		#ifdef IS_EDITOR
		bfu::Event* 		p_ev_MoveWindowArgs = nullptr;
		#endif
		bfu::Event* 		p_ev_MouseMoveEvent = nullptr;
		bfu::Event* 		p_ev_MouseClickEvent = nullptr;
		bfu::Event* 		p_ev_KeyboardEvent = nullptr;

	public:
		ContextBase(bfu::MemBlockBase* memBlock = bfu::StdAllocatorMemBlock::GetMemBlock());
		~ContextBase(){};

/*
		virtual void SwapBuffer() = 0;
		virtual void HandleContextEvents() = 0;
*/

		virtual void Init(const int argc, const char** argv) = 0;
		virtual void SwapBuffer() = 0;
		virtual void MainLoop() = 0;
		virtual void HandleContextEvents() = 0;
		virtual void CleanUp() = 0;
		virtual void GetResolution(uint16_t* X, uint16_t* Y) = 0;
		void SetFPS(double FPS);

/*
		#ifdef IS_EDITOR
		virtual void RenderImGui(){};
		#endif
*/
	};






	
	class ResizeWindowArgs
	{
	public:
		int 	m_width;
		int 	m_height;
		int 	m_Xpos;
		int 	m_Ypos;


		#ifdef IS_EDITOR
		Window 						m_eventSourceWindow = 0;
		#endif


	public:
		ResizeWindowArgs()
		{
			m_width = -1;
			m_height = -1;
			m_Xpos = 0;
			m_Ypos = 0;
		}

		ResizeWindowArgs(const ResizeWindowArgs& copy)
		{
			m_width = copy.m_width;
			m_height = copy.m_height;
			m_Xpos = copy.m_Xpos;
			m_Ypos = copy.m_Ypos;
			#ifdef IS_EDITOR
			m_eventSourceWindow = copy.m_eventSourceWindow;
			#endif
		}
	};

	class MouseMoveEvent
	{
	public:
		int 						m_Xpos;
		int 						m_Ypos;

		#ifdef IS_EDITOR
		int 						m_XposRoot;
		int 						m_YposRoot;
		Window 						m_eventSourceWindow = 0;
		#endif

	public:
		MouseMoveEvent()
		{
			m_Xpos = -1;
			m_Ypos = -1;
		}

		MouseMoveEvent(const MouseMoveEvent& copy)
		{
			m_Xpos = copy.m_Xpos;
			m_Ypos = copy.m_Ypos;
			#ifdef IS_EDITOR
			m_XposRoot = copy.m_XposRoot;
			m_YposRoot = copy.m_YposRoot;
			m_eventSourceWindow = copy.m_eventSourceWindow;
			#endif
		}
	};

	class MouseClickEvent
	{
	public:
		int 						m_Xpos;
		int 						m_Ypos;
		int 						m_key;
		int 						m_state;

		#ifdef IS_EDITOR
		Window 						m_eventSourceWindow = 0;
		#endif

	public:
		MouseClickEvent()
		{
			m_Xpos = -1;
			m_Ypos = -1;
			m_key = (int) asapi::mousecodes::unknown;
			m_state = (int) asapi::keystates::unknown;
		}

		MouseClickEvent(const MouseClickEvent& copy)
		{
			m_Xpos = copy.m_Xpos;
			m_Ypos = copy.m_Ypos;
			m_key = copy.m_key;
			m_state = copy.m_state;
			#ifdef IS_EDITOR
			m_eventSourceWindow = copy.m_eventSourceWindow;
			#endif
		}
	};

	class KeyboardEvent
	{
	public:
		int 						m_key;
		int 						m_state;
		uint8_t 					m_char;

		#ifdef IS_EDITOR
		Window 						m_eventSourceWindow = 0;
		#endif

	public:
		KeyboardEvent()
		{
			m_key = (int) asapi::mousecodes::unknown;
			m_key = (int) asapi::keystates::unknown;
			m_char = (char)0;
		}

		KeyboardEvent(const KeyboardEvent& copy)
		{
			m_key = copy.m_key;
			m_state = copy.m_state;
			m_char = copy.m_char;
			#ifdef IS_EDITOR
			m_eventSourceWindow = copy.m_eventSourceWindow;
			#endif
		}
	};


	#ifdef IS_EDITOR
	class MoveWindowArgs
	{
	public:
		int 						m_X;
		int 						m_Y;


		#ifdef IS_EDITOR
		Window 						m_eventSourceWindow = 0;
		#endif


	public:
		MoveWindowArgs(bfu::MemBlockBase* mBlock = 0)
		{
			m_X = 0;
			m_Y = 0;
		}

		MoveWindowArgs(const MoveWindowArgs& copy)
		{
			m_X = copy.m_X;
			m_Y = copy.m_Y;
			#ifdef IS_EDITOR
			m_eventSourceWindow = copy.m_eventSourceWindow;
			#endif
		}
	};
	#endif
}

#endif
