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

namespace asapgl
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
		EGL_CONTEXT_CLIENT_VERSION, 2,
		//EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE, EGL_TRUE,
        EGL_NONE};

	class ContextBase: public object
	{
	protected:
		bool				m_isRunning = true;
		std::chrono::duration<double> 
							m_frameDelay;


	public:
		ContextBase();
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
		void SetFPS(double FPS);

/*
		#ifdef IS_EDITOR
		virtual void RenderImGui(){};
		#endif
*/
	};






	
	class ResizeWindowArgs: public bfu::EventArgsBase
	{
	public:
		bfu::SerializableVar<int> 	m_width;
		bfu::SerializableVar<int> 	m_height;
		bfu::SerializableVar<int> 	m_Xpos;
		bfu::SerializableVar<int> 	m_Ypos;


		#ifdef IS_EDITOR
		Window 						m_eventSourceWindow = 0;
		#endif


	public:
		ResizeWindowArgs()
			:bfu::EventArgsBase()
			,m_width("m_width",this)
			,m_height("m_height",this)
			,m_Xpos("m_Xpos",this)
			,m_Ypos("m_Ypos",this)
		{
			m_width = -1;
			m_height = -1;
			m_Xpos = 0;
			m_Ypos = 0;
		}

		ResizeWindowArgs(const ResizeWindowArgs& copy)
			:bfu::EventArgsBase()
			,m_width("m_width",this)
			,m_height("m_height",this)
			,m_Xpos("m_Xpos",this)
			,m_Ypos("m_Ypos",this)
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

	class MouseMoveEvent: public bfu::EventArgsBase
	{
	public:
		bfu::SerializableVar<int> m_Xpos;
		bfu::SerializableVar<int> m_Ypos;

		#ifdef IS_EDITOR
		Window 						m_eventSourceWindow = 0;
		#endif

	public:
		MouseMoveEvent()
			:bfu::EventArgsBase()
			,m_Xpos("m_Xpos",this)
			,m_Ypos("m_Ypos",this)
		{
			m_Xpos = -1;
			m_Ypos = -1;
		}

		MouseMoveEvent(const MouseMoveEvent& copy)
			:bfu::EventArgsBase()
			,m_Xpos("m_Xpos",this)
			,m_Ypos("m_Ypos",this)
		{
			m_Xpos = copy.m_Xpos;
			m_Ypos = copy.m_Ypos;
			#ifdef IS_EDITOR
			m_eventSourceWindow = copy.m_eventSourceWindow;
			#endif
		}
	};

	class MouseClickEvent: public bfu::EventArgsBase
	{
	public:
		bfu::SerializableVar<int> m_Xpos;
		bfu::SerializableVar<int> m_Ypos;
		bfu::SerializableVar<int> m_key;
		bfu::SerializableVar<int> m_state;

		#ifdef IS_EDITOR
		Window 						m_eventSourceWindow = 0;
		#endif

	public:
		MouseClickEvent()
			:bfu::EventArgsBase()
			,m_Xpos("m_Xpos",this)
			,m_Ypos("m_Ypos",this)
			,m_key("m_key",this)
			,m_state("m_state",this)
		{
			m_Xpos = -1;
			m_Ypos = -1;
			m_key = (int) asapgl::mousecodes::unknown;
			m_state = (int) asapgl::keystates::unknown;
		}

		MouseClickEvent(const MouseClickEvent& copy)
			:bfu::EventArgsBase()
			,m_Xpos("m_Xpos",this)
			,m_Ypos("m_Ypos",this)
			,m_key("m_key",this)
			,m_state("m_state",this)
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

	class KeyboardEvent: public bfu::EventArgsBase
	{
	public:
		bfu::SerializableVar<int> 		m_key;
		bfu::SerializableVar<int> 		m_state;
		bfu::SerializableVar<uint8_t> 	m_char;

		#ifdef IS_EDITOR
		Window 						m_eventSourceWindow = 0;
		#endif

	public:
		KeyboardEvent()
			:bfu::EventArgsBase()
			,m_key("m_key",this)
			,m_state("m_state",this)
			,m_char("m_char",this)
		{
			m_key = (int) asapgl::mousecodes::unknown;
			m_key = (int) asapgl::keystates::unknown;
			m_char = (char)0;
		}

		KeyboardEvent(const KeyboardEvent& copy)
			:bfu::EventArgsBase()
			,m_key("m_key",this)
			,m_state("m_state",this)
			,m_char("m_char",this)
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
	class MoveWindowArgs: public bfu::EventArgsBase
	{
	public:
		bfu::SerializableVar<int> 	m_X;
		bfu::SerializableVar<int> 	m_Y;


		#ifdef IS_EDITOR
		Window 						m_eventSourceWindow = 0;
		#endif


	public:
		MoveWindowArgs()
			:bfu::EventArgsBase()
			,m_X("m_X",this)
			,m_Y("m_Y",this)
		{
			m_X = 0;
			m_Y = 0;
		}

		MoveWindowArgs(const MoveWindowArgs& copy)
			:bfu::EventArgsBase()
			,m_X("m_X",this)
			,m_Y("m_Y",this)
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
