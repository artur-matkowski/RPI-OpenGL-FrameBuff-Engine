#include "Xlib_EGL_ContextType.hpp"


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <thread>

#include <Systems.hpp>

#include "ImguiXlib.hpp"

#ifdef USE_XLIB
#include <X11/Xatom.h>
#include <X11/Xutil.h>

namespace asapi
{
	


	static asapi::keycodes		m_keyCodeMap[1+(int)asapi::keycodes::unknown] = {asapi::keycodes::unknown};
	static std::map<int, asapi::mousecodes>
								m_mouseCodeMap;

	static char keycodes2char[255] = {0};



	bool Xlib_EGL_ContextType::DisplayOpen(void)
	{
		int screen;
		EGLint major, minor;

		m_XDisplay.display = XOpenDisplay(NULL);
		if (!m_XDisplay.display) {
			return false;
		}

		//XSetCloseDownMode(m_XDisplay.display, RetainPermanent);


		screen = DefaultScreen(m_XDisplay.display);
		m_XDisplay.root = RootWindow(m_XDisplay.display, screen);


		m_XDisplay.egl = eglGetDisplay(m_XDisplay.display);
		if (m_XDisplay.egl == EGL_NO_DISPLAY) {
			XCloseDisplay(m_XDisplay.display);
			return false;
		}

		if (!eglInitialize(m_XDisplay.egl, &major, &minor)) {
			XCloseDisplay(m_XDisplay.display);
			return false;
		}

		log::info << "EGL: " << major << "." << minor << std::endl;



		//printf("EGL: %d.%d\n", major, minor);

		return true;
	}




	Xlib_EGL_ContextType::EGLWindow* Xlib_EGL_ContextType::window_create(const char *name,
					    unsigned int x, unsigned int y,
					    unsigned int width, unsigned int height)
	{
		XSetWindowAttributes attr;
		unsigned long mask;
		XVisualInfo visual;
		EGLint num_configs;
		XVisualInfo* _INFO;
		XSizeHints hints;
		EGLConfig config;
		int num_visuals;
		EGLint version;
		EGLint vid;
		EGLWindow eglWindow;


		if (!eglChooseConfig(m_XDisplay.egl, attributes, &config, 1, &num_configs)) {
			return 0;
		}

		if (!eglGetConfigAttrib(m_XDisplay.egl, config, EGL_NATIVE_VISUAL_ID, &vid)) {
			return 0;
		}

		visual.visualid = vid;

		_INFO = XGetVisualInfo(m_XDisplay.display, VisualIDMask, &visual, &num_visuals);
		if (!_INFO) {
			return 0;
		}

		memset(&attr, 0, sizeof(attr));
		attr.background_pixel = 0;
		attr.border_pixel = 0;
		//attr.override_redirect = true; 
		attr.colormap = XCreateColormap(m_XDisplay.display, m_XDisplay.root, _INFO->visual, AllocNone);
		attr.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask;
		mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;// | CWOverrideRedirect;

		eglWindow.x11 = XCreateWindow(m_XDisplay.display, m_XDisplay.root, x, y, width, height,
					    0, _INFO->depth, InputOutput, _INFO->visual,
					    mask, &attr);
		if (!eglWindow.x11) {
			return 0;
		}

		memset(&hints, 0, sizeof(hints));
		hints.x = x;
		hints.y = y;
		hints.width = width;
		hints.height = height;
		hints.flags = USSize | USPosition;

		XSetNormalHints(m_XDisplay.display, eglWindow.x11, &hints);
		XSetStandardProperties(m_XDisplay.display, eglWindow.x11, name, name, None,
				       NULL, 0, &hints);

		eglBindAPI(EGL_OPENGL_ES_API);

		eglWindow.context = eglCreateContext(m_XDisplay.egl, config,
						   EGL_NO_CONTEXT, contextAttribs);
		if (eglWindow.context == EGL_NO_CONTEXT) {
			return 0;
		}

		eglQueryContext(m_XDisplay.egl, eglWindow.context, EGL_CONTEXT_CLIENT_VERSION, &version);
		log::info << "OpenGL ES: " << version << std::endl;
		
		//printf("OpenGL ES: %d\n", version);

		eglWindow.surface = eglCreateWindowSurface(m_XDisplay.egl, config,
							 eglWindow.x11, NULL);
		if (eglWindow.surface == EGL_NO_SURFACE) {
			return 0;
		}

		XFree(_INFO);

		eglWindow.resolution.x = width;
		eglWindow.resolution.y = height;

		log::debug << "\tegl: " << *(int*)m_XDisplay.egl << " \tcontext: " << *(int*)eglWindow.context << std::endl;

		m_eglWindows.push_back(eglWindow);
		return &m_eglWindows.back();
	}


	void Xlib_EGL_ContextType::window_show(EGLWindow& eglWindow)
	{
	    /* select kind of events we are interested in */
	    XSelectInput(m_XDisplay.display, eglWindow.x11, ExposureMask
				| StructureNotifyMask 
				| SubstructureNotifyMask 
				| KeyPressMask
				| KeyReleaseMask
				//| PointerMotionHintMask
				| ButtonPressMask
				| ButtonReleaseMask
				| PointerMotionMask
				| EnterWindowMask
				| FocusChangeMask);


		if( &eglWindow != m_mainEglWindow )
		{
			Atom window_type;
			long value;

			window_type = XInternAtom(m_XDisplay.display, "_NET_WM_WINDOW_TYPE", False);
			value = XInternAtom(m_XDisplay.display, "_NET_WM_WINDOW_TYPE_DOCK", False);
			XChangeProperty(m_XDisplay.display, eglWindow.x11, window_type, XA_ATOM, 32, PropModeReplace, (unsigned char *) &value, 1);
			
			// window_type = XInternAtom(m_XDisplay.display, "_NET_WM_STATE", False);
			// value = XInternAtom(m_XDisplay.display, "_NET_WM_STATE_HIDDEN", False);
			// XChangeProperty(m_XDisplay.display, eglWindow.x11, window_type, XA_ATOM, 32, PropModeReplace, (unsigned char *) &value, 1);
			
			// window_type = XInternAtom(m_XDisplay.display, "_NET_WM_ALLOWED_ACTIONS", False);
			// value = XInternAtom(m_XDisplay.display, "_NET_WM_ACTION_MOVE", False);
			// XChangeProperty(m_XDisplay.display, eglWindow.x11, window_type, XA_ATOM, 32, PropModeReplace, (unsigned char *) &value, 1);
			
		}

		XMapWindow(m_XDisplay.display, eglWindow.x11);

		if (!eglMakeCurrent(m_XDisplay.egl, eglWindow.surface, eglWindow.surface, eglWindow.context))
			log::error << "eglMakeCurrent()" << std::endl;
			
		XFlush(m_XDisplay.display);
	}


	void Xlib_EGL_ContextType::Init(const int argc, const char** argv)
	{
		const unsigned int width = 1024;
		const unsigned int height = 600;
		bfu::EventSystem& events = SYSTEMS::GetObject().EVENTS;

		m_eglWindows.reserve(8);

		InitMaps();

		DisplayOpen();
		/*
		if (!display) {
			fprintf(stderr, "failed to open display\n");
			return;
		}*/

		m_mainEglWindow = window_create( "Asapi Editor", 50, 100, width, height );
		window_show(*m_mainEglWindow);
		/*if (!window) {
			fprintf(stderr, "failed to create window\n");
			return;
		}*/


		events.Invoke<ResizeWindowArgs>([&](ResizeWindowArgs& args) 
	    {
	    	args.m_width = width; 
	    	args.m_height = height; 
			#ifdef IS_EDITOR
			args.m_eventSourceWindow = m_mainEglWindow->x11;
			#endif
	    });
/*
		bfu::CallbackId id;
	    events.RegisterCallback<KeyboardEvent>(id, [&](bfu::EventArgsBase& a)
	    {
		    KeyboardEvent* args = (KeyboardEvent*)&a;

		    if(args->m_key == (int)asapi::keycodes::snapi_space 
		    	&& args->m_state == (int)asapi::keystates::snapi_down
		    	&& args->m_eventSourceWindow == m_mainEglWindow->x11)
		    {
			    if(renderTgt==0)
			    	renderTgt=1;
			    else
			    	renderTgt=0;

			    log::debug << "new render target window index: " << renderTgt << std::endl;
		    }

	    });
*/

		#ifdef IS_EDITOR
	    // Setup Dear ImGui context
	    IMGUI_CHECKVERSION();
	    ImGui::CreateContext();
	    ImGuiIO& io = ImGui::GetIO(); (void)io;
	    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	    //io.ConfigViewportsNoAutoMerge = true;
	    //io.ConfigViewportsNoTaskBarIcon = true;

	    // Setup Dear ImGui style
	    ImGui::StyleColorsDark();
	    //ImGui::StyleColorsClassic();

	    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	    ImGuiStyle& style = ImGui::GetStyle();
	    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	    {
	        style.WindowRounding = 0.0f;
	        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	    }

	    // Setup Platform/Renderer backends
	    ImGui_ImplXlib_InitForOpenGL(m_mainEglWindow, this);
	    ImGui_ImplOpenGL3_Init();
	    #endif
	}
	
	Xlib_EGL_ContextType::~Xlib_EGL_ContextType()
	{
		#ifdef IS_EDITOR
	    ImGui_ImplXlib_Shutdown();
	    #endif

	    
		XAutoRepeatOn(m_XDisplay.display);

		for(int i=0; i<m_eglWindows.size(); ++i)
		{
			eglDestroySurface(m_XDisplay.egl, m_eglWindows[i].surface);
			eglDestroyContext(m_XDisplay.egl, m_eglWindows[i].context);
			XDestroyWindow(m_XDisplay.display, m_eglWindows[i].x11);
		}

		eglTerminate(m_XDisplay.egl);

		XCloseDisplay(m_XDisplay.display);
	}

	void Xlib_EGL_ContextType::CleanUp()
	{
		m_isRunning = false;
	}


	void Xlib_EGL_ContextType::SwapBuffer()
	{
		//for(int i=0; i<m_eglWindows.size(); ++i)
			eglSwapBuffers(m_XDisplay.egl, m_eglWindows[0].surface);
		glFlush();
	}

	void Xlib_EGL_ContextType::HandleContextEvents()
	{
		XEvent event;
		static bfu::EventSystem& events = SYSTEMS::GetObject().EVENTS;



		while( XPending(m_XDisplay.display) )
		{
			XNextEvent(m_XDisplay.display, &event);
			int key;


			switch (event.type) 
			{
			case DestroyNotify:
				log::debug << "DestroyNotify" << std::endl;
				break;
			case Expose:
				//log::debug << "Expose" << std::endl;
				break;

			case MotionNotify:
				//log::debug << "MotionNotify: " << event.xmotion.x << " " << event.xmotion.y << " " << event.xmotion.window <<  std::endl;
				events.Invoke<MouseMoveEvent>([&](MouseMoveEvent& args) 
			    {
					args.m_Xpos = (int)event.xmotion.x;
					args.m_Ypos = (int)event.xmotion.y;
					#ifdef IS_EDITOR
					args.m_XposRoot = (int)event.xmotion.x_root;
					args.m_YposRoot = (int)event.xmotion.y_root;
			    	args.m_eventSourceWindow = event.xmotion.window;
			    	#endif
			    });
			    for(int i=0; i<m_eglWindows.size(); ++i)
			    {
			    	if( m_eglWindows[i].x11 == event.xmotion.window )
			    	{
			    		m_eglWindows[i].cursorPos.x = (GLint)event.xmotion.x;
			    		m_eglWindows[i].cursorPos.y = (GLint)event.xmotion.y;
			    		break;
			    	}
			    }
				m_focusedWindow = event.xmotion.window;
				break;	

			case ButtonPress:			
				// log::debug << "ButtonPress: " << event.xbutton.x << " " << event.xbutton.y 
				// 	<< " " << event.xbutton.state << " " << event.xbutton.button << std::endl;
				events.Invoke<MouseClickEvent>([&](MouseClickEvent& args) 
			    {
					args.m_Xpos = (int)event.xbutton.x;
					args.m_Ypos = (int)event.xbutton.y;
					#ifdef IS_EDITOR
			    	args.m_eventSourceWindow = event.xbutton.window;
			    	#endif
			    	if(event.xbutton.button==5)
			    	{
						args.m_key = (int)asapi::mousecodes::snapi_wheelY;
						args.m_state = (int)asapi::keystates::snapi_down;
			    	}
			    	else if(event.xbutton.button==4)
			    	{
						args.m_key = (int)asapi::mousecodes::snapi_wheelY;
						args.m_state = (int)asapi::keystates::snapi_up;
			    	}
			    	else
			    	{
						args.m_key = (int)m_mouseCodeMap[event.xbutton.button];
						args.m_state = (int)asapi::keystates::snapi_down;
			    	}
			    });
				break;

			case ButtonRelease:			
				// log::debug << "ButtonRelease: " << event.xbutton.x << " " << event.xbutton.y 
				// 	<< " " << event.xbutton.state << " " << event.xbutton.button << std::endl;
				events.Invoke<MouseClickEvent>([&](MouseClickEvent& args) 
			    {
					args.m_Xpos = (int)event.xbutton.x;
					args.m_Ypos = (int)event.xbutton.y;
					args.m_key = (int)m_mouseCodeMap[event.xbutton.button];
					args.m_state = (int)asapi::keystates::snapi_up;
					#ifdef IS_EDITOR
			    	args.m_eventSourceWindow = event.xbutton.window;
			    	#endif
			    });
				break;

			case ConfigureNotify:
				//log::debug << "ConfigureNotify: " << event.xconfigure.x << " " << event.xconfigure.y << " \\ " << event.xconfigure.width << " " << event.xconfigure.height << std::endl;
			    events.Invoke<ResizeWindowArgs>([&](ResizeWindowArgs& args) 
			    {
			    	args.m_width = (GLint)event.xconfigure.width; 
			    	args.m_height = (GLint)event.xconfigure.height; 
			    	args.m_Xpos = (GLint)event.xconfigure.x; 
			    	args.m_Ypos = (GLint)event.xconfigure.y; 
					#ifdef IS_EDITOR
			    	args.m_eventSourceWindow = event.xconfigure.window;
			    	#endif
			    });
			    for(int i=0; i<m_eglWindows.size(); ++i)
			    {
			    	if( m_eglWindows[i].x11 == event.xconfigure.window )
			    	{
			    		m_eglWindows[i].resolution.x = (GLint)event.xconfigure.width;
			    		m_eglWindows[i].resolution.y = (GLint)event.xconfigure.height;
			    		m_eglWindows[i].position.x = (GLint)event.xconfigure.x;
			    		m_eglWindows[i].position.y = (GLint)event.xconfigure.y;
			    		break;
			    	}
			    }
				break;

			case KeyPress:
				//log::debug << "inputed key keycode: " << event.xkey.keycode << std::endl;
				key = (int)asapi::keycodes::unknown;

				if(event.xkey.keycode < (int)asapi::keycodes::unknown){
					key = (int)m_keyCodeMap[(int)event.xkey.keycode];
				}

				events.Invoke<KeyboardEvent>([&](KeyboardEvent& args) 
			    {
			    	args.m_key = (int)key; 
			    	args.m_state = (int)asapi::keystates::snapi_down; 
			    	args.m_char = (char)keycodes2char[ key ];
					#ifdef IS_EDITOR
			    	args.m_eventSourceWindow = event.xkey.window;
			    	#endif
			    });
				break;

			case KeyRelease:
				key = (int)asapi::keycodes::unknown;

				if(event.xkey.keycode < (int)asapi::keycodes::unknown){
					key = (int)m_keyCodeMap[(int)event.xkey.keycode];
				}

				events.Invoke<KeyboardEvent>([&](KeyboardEvent& args) 
			    {
			    	args.m_key = (int)key; 
			    	args.m_state = (int)asapi::keystates::snapi_up; 
			    	args.m_char = (char)keycodes2char[ key ];
					#ifdef IS_EDITOR
			    	args.m_eventSourceWindow = event.xkey.window;
			    	#endif
			    });
			    break;
			
			case FocusIn:
				//log::debug << "FocusIn: " << event.xfocus.window << std::endl;
				XAutoRepeatOff(m_XDisplay.display);
			    break;

			case FocusOut:
				XAutoRepeatOn(m_XDisplay.display);
				//m_focusedWindow = 0;
			    break;

			default:
				break;
			}
		
		}
	}


	#ifdef IS_EDITOR
	void Xlib_EGL_ContextType::RenderImGui()
	{
		/*
		EGLDisplay egl = eglGetCurrentDisplay();
		EGLContext context = eglGetCurrentContext();
		//log::debug << "\tegl: " << *(int*)egl << " \tcontext: " << *(int*)context << std::endl;
		//if (!eglMakeCurrent(m_eglWindows[renderTgt].egl, m_eglWindows[renderTgt].surface, m_eglWindows[renderTgt].surface, m_eglWindows[renderTgt].context))
		//if (!eglMakeCurrent(eglGetCurrentDisplay(), m_eglWindows[renderTgt].surface, m_eglWindows[renderTgt].surface, eglGetCurrentContext()))
		if (!eglMakeCurrent(egl, m_eglWindows[renderTgt].surface, m_eglWindows[renderTgt].surface, context))
			log::error << "eglMakeCurrent():\n" << std::endl;
			*/
	}
	#endif

	void Xlib_EGL_ContextType::MainLoop()
	{
		SYSTEMS& system = SYSTEMS::GetObject();
		bfu::EventSystem& events = system.EVENTS;
		RendererSystem& rendererSystem = system.RENDERER;

		auto frameEnd =  std::chrono::system_clock::now();
		auto frameStart = std::chrono::high_resolution_clock::now();
		bool show_demo_window = true;
		bool show_another_window = true;
		std::chrono::duration<double> frameDeltaTime( m_frameDelay );

		#ifdef IS_EDITOR
    	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    	ImGuiIO& io = ImGui::GetIO(); (void)io;    	
		Mesh 			cursorMesh( glm::vec2(m_mainEglWindow->resolution.x, m_mainEglWindow->resolution.y) );
		MaterialType 	cursorMaterial("cursor");
		Uniform<glm::vec3>* uCursorPos = (Uniform<glm::vec3>*)cursorMaterial.GetUniformPtr("offset");
		#endif

		std::chrono::duration<double> elapsed;

		GLfloat rotation = 0.0;


		while(m_isRunning)
		{
			frameStart = std::chrono::high_resolution_clock::now();

			HandleContextEvents();

			//TODO frame stuff
			{
				rotation += frameDeltaTime.count();

				//eglMakeCurrent(m_XDisplay.egl, m_mainEglWindow->surface, m_mainEglWindow->surface, m_mainEglWindow->context);

		        glViewport(0, 0, m_mainEglWindow->resolution.x, m_mainEglWindow->resolution.y);
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				rendererSystem.Render();


				#ifdef IS_EDITOR


				// glViewport(0, 0, m_mainEglWindow->resolution.x, m_mainEglWindow->resolution.y);
				// glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				// glClear(GL_COLOR_BUFFER_BIT);
				
		        // Start the Dear ImGui frame
		        ImGui_ImplOpenGL3_NewFrame();
		        ImGui_ImplXlib_NewFrame();
       			ImGui::NewFrame();


				system.OnGUI();

		        // Rendering
		        ImGui::Render();

		        // If you are using this code with non-legacy OpenGL header/contexts (which you should not, prefer using imgui_impl_opengl3.cpp!!),
		        // you may need to backup/reset/restore current shader using the commented lines below.
		        //GLint last_program;
		        //glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
		        //glUseProgram(0);
		        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		        //glUseProgram(last_program);

		        // Update and Render additional Platform Windows
		        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		        {
		            //GLFWwindow* backup_current_context = glfwGetCurrentContext();
		            ImGui::UpdatePlatformWindows();
		            ImGui::RenderPlatformWindowsDefault();
		            eglMakeCurrent(m_XDisplay.egl, m_mainEglWindow->surface, m_mainEglWindow->surface, m_mainEglWindow->context);
		        }
		        

				glm::vec2 mousePos(m_mainEglWindow->cursorPos.x / (float)m_mainEglWindow->resolution.x*2.0f - 1.0f
					, 1.0f - m_mainEglWindow->cursorPos.y / (float)m_mainEglWindow->resolution.y*2.0f );



				cursorMaterial.BindMaterial();
				uCursorPos->SetUniform(glm::vec3(mousePos.x, mousePos.y, 0.0f));
				cursorMesh.Render();
				#endif
				
				SwapBuffer();
			}





			std::chrono::duration<double> calculationTime = std::chrono::high_resolution_clock::now() - frameStart;
			std::chrono::duration<double> diffToFrameEnd = m_frameDelay - calculationTime;


			#ifdef IS_EDITOR
		    // Setup time step
		    io.DeltaTime = (float)frameDeltaTime.count();
			#endif



			//log::debug << "frameDeltaTime: "  << (float)frameDeltaTime.count() << "s, Calculation time: " << (float)calculationTime.count() << "s" << std::endl;

			std::this_thread::sleep_for(diffToFrameEnd);

			frameEnd = std::chrono::high_resolution_clock::now();
			frameDeltaTime = frameEnd - frameStart;
		}		
	}


	void Xlib_EGL_ContextType::InitMaps()
	{


	   	m_mouseCodeMap[ 1 ]							= asapi::mousecodes::snapi_left;
	   	m_mouseCodeMap[ 3 ]							= asapi::mousecodes::snapi_right;
	   	m_mouseCodeMap[ 2 ]							= asapi::mousecodes::snapi_middle;
	   	//m_mouseCodeMap[ BTN_BACK ]				= asapi::mousecodes::snapi_back;
	   	//m_mouseCodeMap[ BTN_FORWARD ]				= asapi::mousecodes::snapi_forward;

	   	m_keyCodeMap[ 0 ]							= asapi::keycodes::snapi_reserved;
		m_keyCodeMap[ 9 ]							= asapi::keycodes::snapi_esc;
		m_keyCodeMap[ 10 ]							= asapi::keycodes::snapi_1;
		m_keyCodeMap[ 11 ]							= asapi::keycodes::snapi_2;
		m_keyCodeMap[ 12 ]							= asapi::keycodes::snapi_3;
		m_keyCodeMap[ 13 ]							= asapi::keycodes::snapi_4;
		m_keyCodeMap[ 14 ]							= asapi::keycodes::snapi_5;
		m_keyCodeMap[ 15 ]							= asapi::keycodes::snapi_6;
		m_keyCodeMap[ 16 ]							= asapi::keycodes::snapi_7;
		m_keyCodeMap[ 17 ]							= asapi::keycodes::snapi_8;
		m_keyCodeMap[ 18 ]							= asapi::keycodes::snapi_9;
		m_keyCodeMap[ 19 ]							= asapi::keycodes::snapi_0;
		m_keyCodeMap[ 20 ]							= asapi::keycodes::snapi_minus;
		m_keyCodeMap[ 21 ]							= asapi::keycodes::snapi_equal;
		m_keyCodeMap[ 22 ]							= asapi::keycodes::snapi_backspace;
		m_keyCodeMap[ 23 ]							= asapi::keycodes::snapi_tab;
		m_keyCodeMap[ 24 ]							= asapi::keycodes::snapi_q;
		m_keyCodeMap[ 25 ]							= asapi::keycodes::snapi_w;
		m_keyCodeMap[ 26 ]							= asapi::keycodes::snapi_e;
		m_keyCodeMap[ 27 ]							= asapi::keycodes::snapi_r;
		m_keyCodeMap[ 28 ]							= asapi::keycodes::snapi_t;
		m_keyCodeMap[ 29 ]							= asapi::keycodes::snapi_y;
		m_keyCodeMap[ 30 ]							= asapi::keycodes::snapi_u;
		m_keyCodeMap[ 31 ]							= asapi::keycodes::snapi_i;
		m_keyCodeMap[ 32 ]							= asapi::keycodes::snapi_o;
		m_keyCodeMap[ 33 ]							= asapi::keycodes::snapi_p;
		m_keyCodeMap[ 34 ]							= asapi::keycodes::snapi_leftbrace;
		m_keyCodeMap[ 35 ]							= asapi::keycodes::snapi_rightbrace;
		m_keyCodeMap[ 36 ]							= asapi::keycodes::snapi_enter;
		m_keyCodeMap[ 37 ]							= asapi::keycodes::snapi_leftctrl;
		m_keyCodeMap[ 38 ]							= asapi::keycodes::snapi_a;
		m_keyCodeMap[ 39 ]							= asapi::keycodes::snapi_s;
		m_keyCodeMap[ 40 ]							= asapi::keycodes::snapi_d;
		m_keyCodeMap[ 41 ]							= asapi::keycodes::snapi_f;
		m_keyCodeMap[ 42 ]							= asapi::keycodes::snapi_g;
		m_keyCodeMap[ 43 ]							= asapi::keycodes::snapi_h;
		m_keyCodeMap[ 44 ]							= asapi::keycodes::snapi_j;
		m_keyCodeMap[ 45 ]							= asapi::keycodes::snapi_k;
		m_keyCodeMap[ 46 ]							= asapi::keycodes::snapi_l;
		m_keyCodeMap[ 47 ]							= asapi::keycodes::snapi_semicolon;
		m_keyCodeMap[ 48 ]							= asapi::keycodes::snapi_apostrophe;
		m_keyCodeMap[ 49 ]							= asapi::keycodes::snapi_grave;
		m_keyCodeMap[ 50 ]							= asapi::keycodes::snapi_leftshift;
		m_keyCodeMap[ 51 ]							= asapi::keycodes::snapi_backslash;
		m_keyCodeMap[ 52 ]							= asapi::keycodes::snapi_z;
		m_keyCodeMap[ 53 ]							= asapi::keycodes::snapi_x;
		m_keyCodeMap[ 54 ]							= asapi::keycodes::snapi_c;
		m_keyCodeMap[ 55 ]							= asapi::keycodes::snapi_v;
		m_keyCodeMap[ 56 ]							= asapi::keycodes::snapi_b;
		m_keyCodeMap[ 57 ]							= asapi::keycodes::snapi_n;
		m_keyCodeMap[ 58 ]							= asapi::keycodes::snapi_m;
		m_keyCodeMap[ 59 ]							= asapi::keycodes::snapi_comma;
		m_keyCodeMap[ 60 ]							= asapi::keycodes::snapi_dot;
		m_keyCodeMap[ 61 ]							= asapi::keycodes::snapi_slash;
		m_keyCodeMap[ 62 ]							= asapi::keycodes::snapi_rightshift;
		m_keyCodeMap[ 63 ]							= asapi::keycodes::snapi_kpasterisk;
		m_keyCodeMap[ 64 ]							= asapi::keycodes::snapi_leftalt;
		m_keyCodeMap[ 65 ]							= asapi::keycodes::snapi_space;
		m_keyCodeMap[ 66 ]							= asapi::keycodes::snapi_capslock;
		m_keyCodeMap[ 67 ]							= asapi::keycodes::snapi_f1;
		m_keyCodeMap[ 68 ]							= asapi::keycodes::snapi_f2;
		m_keyCodeMap[ 69 ]							= asapi::keycodes::snapi_f3;
		m_keyCodeMap[ 70 ]							= asapi::keycodes::snapi_f4;
		m_keyCodeMap[ 71 ]							= asapi::keycodes::snapi_f5;
		m_keyCodeMap[ 72 ]							= asapi::keycodes::snapi_f6;
		m_keyCodeMap[ 73 ]							= asapi::keycodes::snapi_f7;
		m_keyCodeMap[ 74 ]							= asapi::keycodes::snapi_f8;
		m_keyCodeMap[ 75 ]							= asapi::keycodes::snapi_f9;
		m_keyCodeMap[ 76 ]							= asapi::keycodes::snapi_f10;
		m_keyCodeMap[ 77 ]							= asapi::keycodes::snapi_numlock;
		//m_keyCodeMap[ 88 ]						= asapi::keycodes::snapi_scrolllock;
		m_keyCodeMap[ 79 ]							= asapi::keycodes::snapi_kp7;
		m_keyCodeMap[ 80 ]							= asapi::keycodes::snapi_kp8;
		m_keyCodeMap[ 81 ]							= asapi::keycodes::snapi_kp9;
		m_keyCodeMap[ 82 ]							= asapi::keycodes::snapi_kpminus;
		m_keyCodeMap[ 83 ]							= asapi::keycodes::snapi_kp4;
		m_keyCodeMap[ 84 ]							= asapi::keycodes::snapi_kp5;
		m_keyCodeMap[ 85 ]							= asapi::keycodes::snapi_kp6;
		m_keyCodeMap[ 86 ]							= asapi::keycodes::snapi_kpplus;
		m_keyCodeMap[ 87 ]							= asapi::keycodes::snapi_kp1;
		m_keyCodeMap[ 88 ]							= asapi::keycodes::snapi_kp2;
		m_keyCodeMap[ 89 ]							= asapi::keycodes::snapi_kp3;
		m_keyCodeMap[ 90 ]							= asapi::keycodes::snapi_kp0;
		m_keyCodeMap[ 91 ]							= asapi::keycodes::snapi_kpdot;
		//m_keyCodeMap[ 92 ]		= asapi::keycodes::snapi_zenkakuhankaku;
		//m_keyCodeMap[ 93 ]					= asapi::keycodes::snapi_102nd;
		//m_keyCodeMap[ 94 ]					= asapi::keycodes::snapi_f11;
		//m_keyCodeMap[ 95 ]					= asapi::keycodes::snapi_f12;
		//m_keyCodeMap[ 96 ]					= asapi::keycodes::snapi_ro;
		//m_keyCodeMap[ 97 ]				= asapi::keycodes::snapi_katakana;
		//m_keyCodeMap[ 98 ]				= asapi::keycodes::snapi_hiragana;
		//m_keyCodeMap[ 99 ]				= asapi::keycodes::snapi_henkan;
		//m_keyCodeMap[ 100 ]		= asapi::keycodes::snapi_katakanahiragana;
		//m_keyCodeMap[ 101 ]				= asapi::keycodes::snapi_muhenkan;
		//m_keyCodeMap[ 102 ]				= asapi::keycodes::snapi_kpjpcomma;
		m_keyCodeMap[ 104 ]							= asapi::keycodes::snapi_kpenter;
		m_keyCodeMap[ 105 ]							= asapi::keycodes::snapi_rightctrl;
		m_keyCodeMap[ 106 ]							= asapi::keycodes::snapi_kpslash;
		//m_keyCodeMap[ 107 ]						= asapi::keycodes::snapi_sysrq;
		m_keyCodeMap[ 108 ]							= asapi::keycodes::snapi_rightalt;
		//m_keyCodeMap[ 109 ]						= asapi::keycodes::snapi_linefeed;
		m_keyCodeMap[ 110 ]							= asapi::keycodes::snapi_home;
		m_keyCodeMap[ 111 ]							= asapi::keycodes::snapi_up;
		m_keyCodeMap[ 112 ]							= asapi::keycodes::snapi_pageup;
		m_keyCodeMap[ 113 ]							= asapi::keycodes::snapi_left;
		m_keyCodeMap[ 114 ]							= asapi::keycodes::snapi_right;
		m_keyCodeMap[ 115 ]							= asapi::keycodes::snapi_end;
		m_keyCodeMap[ 116 ]							= asapi::keycodes::snapi_down;
		m_keyCodeMap[ 117 ]							= asapi::keycodes::snapi_pagedown;
		m_keyCodeMap[ 118 ]							= asapi::keycodes::snapi_insert;
		m_keyCodeMap[ 119 ]							= asapi::keycodes::snapi_delete;
		//m_keyCodeMap[ KEY_MACRO ]					= asapi::keycodes::snapi_macro;
		//m_keyCodeMap[ KEY_MUTE ]					= asapi::keycodes::snapi_mute;
		//m_keyCodeMap[ KEY_VOLUMEDOWN ]				= asapi::keycodes::snapi_volumedown;
		//m_keyCodeMap[ KEY_VOLUMEUP ]				= asapi::keycodes::snapi_volumeup;
		//m_keyCodeMap[ KEY_POWER ]					= asapi::keycodes::snapi_power;
		//m_keyCodeMap[ KEY_KPEQUAL ]					= asapi::keycodes::snapi_kpequal;
		//m_keyCodeMap[ KEY_KPPLUSMINUS ]				= asapi::keycodes::snapi_kpplusminus;
		//m_keyCodeMap[ KEY_PAUSE ]					= asapi::keycodes::snapi_pause;
		//m_keyCodeMap[ KEY_SCALE ]					= asapi::keycodes::snapi_scale;
		//m_keyCodeMap[ KEY_KPCOMMA ]					= asapi::keycodes::snapi_kpcomma;
		//m_keyCodeMap[ KEY_HANGEUL ]					= asapi::keycodes::snapi_hangeul;
		//m_keyCodeMap[ KEY_HANGUEL ]					= asapi::keycodes::snapi_hanguel;
		//m_keyCodeMap[ KEY_HANJA ]					= asapi::keycodes::snapi_hanja;
		//m_keyCodeMap[ KEY_YEN ]						= asapi::keycodes::snapi_yen;
		//m_keyCodeMap[ KEY_LEFTMETA ]				= asapi::keycodes::snapi_leftmeta;
		//m_keyCodeMap[ KEY_RIGHTMETA ]				= asapi::keycodes::snapi_rightmeta;
		//m_keyCodeMap[ KEY_COMPOSE ]					= asapi::keycodes::snapi_compose;
		//m_keyCodeMap[ KEY_STOP ]					= asapi::keycodes::snapi_stop;
		//m_keyCodeMap[ KEY_AGAIN ]					= asapi::keycodes::snapi_again;
		//m_keyCodeMap[ KEY_PROPS ]					= asapi::keycodes::snapi_props;
		//m_keyCodeMap[ KEY_UNDO ]					= asapi::keycodes::snapi_undo;
		//m_keyCodeMap[ KEY_FRONT ]					= asapi::keycodes::snapi_front;
		//m_keyCodeMap[ KEY_COPY ]					= asapi::keycodes::snapi_copy;
		//m_keyCodeMap[ KEY_OPEN ]					= asapi::keycodes::snapi_open;
		//m_keyCodeMap[ KEY_PASTE ]					= asapi::keycodes::snapi_paste;
		//m_keyCodeMap[ KEY_FIND ]					= asapi::keycodes::snapi_find;
		//m_keyCodeMap[ KEY_CUT ]						= asapi::keycodes::snapi_cut;
		//m_keyCodeMap[ KEY_HELP ]					= asapi::keycodes::snapi_help;
		//m_keyCodeMap[ KEY_MENU ]					= asapi::keycodes::snapi_menu;
		//m_keyCodeMap[ KEY_CALC ]					= asapi::keycodes::snapi_calc;
		//m_keyCodeMap[ KEY_SETUP ]					= asapi::keycodes::snapi_setup;
		//m_keyCodeMap[ KEY_SLEEP ]					= asapi::keycodes::snapi_sleep;
		//m_keyCodeMap[ KEY_WAKEUP ]					= asapi::keycodes::snapi_wakeup;
		//m_keyCodeMap[ KEY_FILE ]					= asapi::keycodes::snapi_file;
		//m_keyCodeMap[ KEY_SENDFILE ]				= asapi::keycodes::snapi_sendfile;
		//m_keyCodeMap[ KEY_DELETEFILE ]				= asapi::keycodes::snapi_deletefile;
		//m_keyCodeMap[ KEY_XFER ]					= asapi::keycodes::snapi_xfer;
		//m_keyCodeMap[ KEY_PROG1 ]					= asapi::keycodes::snapi_prog1;
		//m_keyCodeMap[ KEY_PROG2 ]					= asapi::keycodes::snapi_prog2;
		//m_keyCodeMap[ KEY_WWW ]						= asapi::keycodes::snapi_www;
		//m_keyCodeMap[ KEY_MSDOS ]					= asapi::keycodes::snapi_msdos;
		//m_keyCodeMap[ KEY_COFFEE ]					= asapi::keycodes::snapi_coffee;
		//m_keyCodeMap[ KEY_SCREENLOCK ]				= asapi::keycodes::snapi_screenlock;
		//m_keyCodeMap[ KEY_ROTATE_DISPLAY ]			= asapi::keycodes::snapi_rotate_display;
		//m_keyCodeMap[ KEY_DIRECTION ]				= asapi::keycodes::snapi_direction;
		//m_keyCodeMap[ KEY_CYCLEWINDOWS ]			= asapi::keycodes::snapi_cyclewindows;
		//m_keyCodeMap[ KEY_MAIL ]					= asapi::keycodes::snapi_mail;
		//m_keyCodeMap[ KEY_BOOKMARKS ]				= asapi::keycodes::snapi_bookmarks;
		//m_keyCodeMap[ KEY_COMPUTER ]				= asapi::keycodes::snapi_computer;
		//m_keyCodeMap[ KEY_BACK ]					= asapi::keycodes::snapi_back;
		//m_keyCodeMap[ KEY_FORWARD ]					= asapi::keycodes::snapi_forward;
		//m_keyCodeMap[ KEY_CLOSECD ]					= asapi::keycodes::snapi_closecd;
		//m_keyCodeMap[ KEY_EJECTCD ]					= asapi::keycodes::snapi_ejectcd;
		//m_keyCodeMap[ KEY_EJECTCLOSECD ]			= asapi::keycodes::snapi_ejectclosecd;
		//m_keyCodeMap[ KEY_NEXTSONG ]				= asapi::keycodes::snapi_nextsong;
		//m_keyCodeMap[ KEY_PLAYPAUSE ]				= asapi::keycodes::snapi_playpause;
		//m_keyCodeMap[ KEY_PREVIOUSSONG ]			= asapi::keycodes::snapi_previoussong;
		//m_keyCodeMap[ KEY_STOPCD ]					= asapi::keycodes::snapi_stopcd;
		//m_keyCodeMap[ KEY_RECORD ]					= asapi::keycodes::snapi_record;
		//m_keyCodeMap[ KEY_REWIND ]					= asapi::keycodes::snapi_rewind;
		//m_keyCodeMap[ KEY_PHONE ]					= asapi::keycodes::snapi_phone;
		//m_keyCodeMap[ KEY_ISO ]						= asapi::keycodes::snapi_iso;
		//m_keyCodeMap[ KEY_CONFIG ]					= asapi::keycodes::snapi_config;
		//m_keyCodeMap[ KEY_HOMEPAGE ]				= asapi::keycodes::snapi_homepage;
		//m_keyCodeMap[ KEY_REFRESH ]					= asapi::keycodes::snapi_refresh;
		//m_keyCodeMap[ KEY_EXIT ]					= asapi::keycodes::snapi_exit;
		//m_keyCodeMap[ KEY_MOVE ]					= asapi::keycodes::snapi_move;
		//m_keyCodeMap[ KEY_EDIT ]					= asapi::keycodes::snapi_edit;
		//m_keyCodeMap[ KEY_SCROLLUP ]				= asapi::keycodes::snapi_scrollup;
		//m_keyCodeMap[ KEY_SCROLLDOWN ]				= asapi::keycodes::snapi_scrolldown;
		//m_keyCodeMap[ KEY_KPLEFTPAREN ]				= asapi::keycodes::snapi_kpleftparen;
		//m_keyCodeMap[ KEY_KPRIGHTPAREN ]			= asapi::keycodes::snapi_kprightparen;
		//m_keyCodeMap[ KEY_NEW ]						= asapi::keycodes::snapi_new;
		//m_keyCodeMap[ KEY_REDO ]					= asapi::keycodes::snapi_redo;
		//m_keyCodeMap[ KEY_F13 ]						= asapi::keycodes::snapi_f13;
		//m_keyCodeMap[ KEY_F14 ]						= asapi::keycodes::snapi_f14;
		//m_keyCodeMap[ KEY_F15 ]						= asapi::keycodes::snapi_f15;
		//m_keyCodeMap[ KEY_F16 ]						= asapi::keycodes::snapi_f16;
		//m_keyCodeMap[ KEY_F17 ]						= asapi::keycodes::snapi_f17;
		//m_keyCodeMap[ KEY_F18 ]						= asapi::keycodes::snapi_f18;
		//m_keyCodeMap[ KEY_F19 ]						= asapi::keycodes::snapi_f19;
		//m_keyCodeMap[ KEY_F20 ]						= asapi::keycodes::snapi_f20;
		//m_keyCodeMap[ KEY_F21 ]						= asapi::keycodes::snapi_f21;
		//m_keyCodeMap[ KEY_F22 ]						= asapi::keycodes::snapi_f22;
		//m_keyCodeMap[ KEY_F23 ]						= asapi::keycodes::snapi_f23;
		//m_keyCodeMap[ KEY_F24 ]						= asapi::keycodes::snapi_f24;
		//m_keyCodeMap[ KEY_PLAYCD ]					= asapi::keycodes::snapi_playcd;
		//m_keyCodeMap[ KEY_PAUSECD ]					= asapi::keycodes::snapi_pausecd;
		//m_keyCodeMap[ KEY_PROG3 ]					= asapi::keycodes::snapi_prog3;
		//m_keyCodeMap[ KEY_PROG4 ]					= asapi::keycodes::snapi_prog4;
		//m_keyCodeMap[ KEY_DASHBOARD ]				= asapi::keycodes::snapi_dashboard;
		//m_keyCodeMap[ KEY_SUSPEND ]					= asapi::keycodes::snapi_suspend;
		//m_keyCodeMap[ KEY_CLOSE ]					= asapi::keycodes::snapi_close;
		//m_keyCodeMap[ KEY_PLAY ]					= asapi::keycodes::snapi_play;
		//m_keyCodeMap[ KEY_FASTFORWARD ]				= asapi::keycodes::snapi_fastforward;
		//m_keyCodeMap[ KEY_BASSBOOST ]				= asapi::keycodes::snapi_bassboost;
		//m_keyCodeMap[ KEY_PRINT ]					= asapi::keycodes::snapi_print;
		//m_keyCodeMap[ KEY_HP ]						= asapi::keycodes::snapi_hp;
		//m_keyCodeMap[ KEY_CAMERA ]					= asapi::keycodes::snapi_camera;
		//m_keyCodeMap[ KEY_SOUND ]					= asapi::keycodes::snapi_sound;
		//m_keyCodeMap[ KEY_QUESTION ]				= asapi::keycodes::snapi_question;
		//m_keyCodeMap[ KEY_EMAIL ]					= asapi::keycodes::snapi_email;
		//m_keyCodeMap[ KEY_CHAT ]					= asapi::keycodes::snapi_chat;
		//m_keyCodeMap[ KEY_SEARCH ]					= asapi::keycodes::snapi_search;
		//m_keyCodeMap[ KEY_CONNECT ]					= asapi::keycodes::snapi_connect;
		//m_keyCodeMap[ KEY_FINANCE ]					= asapi::keycodes::snapi_finance;
		//m_keyCodeMap[ KEY_SPORT ]					= asapi::keycodes::snapi_sport;
		//m_keyCodeMap[ KEY_SHOP ]					= asapi::keycodes::snapi_shop;
		//m_keyCodeMap[ KEY_ALTERASE ]				= asapi::keycodes::snapi_alterase;
		//m_keyCodeMap[ KEY_CANCEL ]					= asapi::keycodes::snapi_cancel;
		//m_keyCodeMap[ KEY_BRIGHTNESSDOWN ]			= asapi::keycodes::snapi_brightnessdown;
		//m_keyCodeMap[ KEY_BRIGHTNESSUP ]			= asapi::keycodes::snapi_brightnessup;
		//m_keyCodeMap[ KEY_MEDIA ]					= asapi::keycodes::snapi_media;
		//m_keyCodeMap[ KEY_SWITCHVIDEOMODE ]			= asapi::keycodes::snapi_switchvideomode;
		//m_keyCodeMap[ KEY_KBDILLUMTOGGLE ]			= asapi::keycodes::snapi_kbdillumtoggle;
		//m_keyCodeMap[ KEY_KBDILLUMDOWN ]			= asapi::keycodes::snapi_kbdillumdown;
		//m_keyCodeMap[ KEY_KBDILLUMUP ]				= asapi::keycodes::snapi_kbdillumup;
		//m_keyCodeMap[ KEY_SEND ]					= asapi::keycodes::snapi_send;
		//m_keyCodeMap[ KEY_REPLY ]					= asapi::keycodes::snapi_reply;
		//m_keyCodeMap[ KEY_FORWARDMAIL ]				= asapi::keycodes::snapi_forwardmail;
		//m_keyCodeMap[ KEY_SAVE ]					= asapi::keycodes::snapi_save;
		//m_keyCodeMap[ KEY_DOCUMENTS ]				= asapi::keycodes::snapi_documents;
		//m_keyCodeMap[ KEY_BATTERY ]					= asapi::keycodes::snapi_battery;
		//m_keyCodeMap[ KEY_BLUETOOTH ]				= asapi::keycodes::snapi_bluetooth;
		//m_keyCodeMap[ KEY_WLAN ]					= asapi::keycodes::snapi_wlan;
		//m_keyCodeMap[ KEY_UWB ]						= asapi::keycodes::snapi_uwb;
		//m_keyCodeMap[ KEY_UNKNOWN ]					= asapi::keycodes::snapi_unknown;
		//m_keyCodeMap[ KEY_VIDEO_NEXT ]				= asapi::keycodes::snapi_video_next;
		//m_keyCodeMap[ KEY_VIDEO_PREV ]				= asapi::keycodes::snapi_video_prev;
		//m_keyCodeMap[ KEY_BRIGHTNESS_CYCLE ]		= asapi::keycodes::snapi_brightness_cycle;
		//m_keyCodeMap[ KEY_BRIGHTNESS_AUTO ]			= asapi::keycodes::snapi_brightness_auto;
		//m_keyCodeMap[ KEY_BRIGHTNESS_ZERO ]			= asapi::keycodes::snapi_brightness_zero;
		//m_keyCodeMap[ KEY_DISPLAY_OFF ]				= asapi::keycodes::snapi_display_off;
		//m_keyCodeMap[ KEY_WWAN ]					= asapi::keycodes::snapi_wwan;
		//m_keyCodeMap[ KEY_WIMAX ]					= asapi::keycodes::snapi_wimax;
		//m_keyCodeMap[ KEY_RFKILL ]					= asapi::keycodes::snapi_rfkill;
		//m_keyCodeMap[ KEY_MICMUTE ]					= asapi::keycodes::snapi_micmute;




		keycodes2char[ (int) asapi::keycodes::snapi_esc ]						= (char)27; //escape
		keycodes2char[ (int) asapi::keycodes::snapi_1 ]						= '1';
		keycodes2char[ (int) asapi::keycodes::snapi_2 ]						= '2';
		keycodes2char[ (int) asapi::keycodes::snapi_3 ]						= '3';
		keycodes2char[ (int) asapi::keycodes::snapi_4 ]						= '4';
		keycodes2char[ (int) asapi::keycodes::snapi_5 ]						= '5';
		keycodes2char[ (int) asapi::keycodes::snapi_6 ]						= '6';
		keycodes2char[ (int) asapi::keycodes::snapi_7 ]						= '7';
		keycodes2char[ (int) asapi::keycodes::snapi_8 ]						= '8';
		keycodes2char[ (int) asapi::keycodes::snapi_9 ]						= '9';
		keycodes2char[ (int) asapi::keycodes::snapi_0 ]						= '0';
		keycodes2char[ (int) asapi::keycodes::snapi_minus ]					= '-';
		keycodes2char[ (int) asapi::keycodes::snapi_equal ]					= '=';
		keycodes2char[ (int) asapi::keycodes::snapi_backspace ]				= (char)8; //backspace
		keycodes2char[ (int) asapi::keycodes::snapi_q ]						= 'q';
		keycodes2char[ (int) asapi::keycodes::snapi_w ]						= 'w';
		keycodes2char[ (int) asapi::keycodes::snapi_e ]						= 'e';
		keycodes2char[ (int) asapi::keycodes::snapi_r ]						= 'r';
		keycodes2char[ (int) asapi::keycodes::snapi_t ]						= 't';
		keycodes2char[ (int) asapi::keycodes::snapi_y ]						= 'y';
		keycodes2char[ (int) asapi::keycodes::snapi_u ]						= 'u';
		keycodes2char[ (int) asapi::keycodes::snapi_i ]						= 'i';
		keycodes2char[ (int) asapi::keycodes::snapi_o ]						= 'o';
		keycodes2char[ (int) asapi::keycodes::snapi_p ]						= 'p';
		keycodes2char[ (int) asapi::keycodes::snapi_leftbrace ]				= '{';
		keycodes2char[ (int) asapi::keycodes::snapi_rightbrace ]				= '}';
		keycodes2char[ (int) asapi::keycodes::snapi_enter ]					= '\n';
		keycodes2char[ (int) asapi::keycodes::snapi_a ]						= 'a';
		keycodes2char[ (int) asapi::keycodes::snapi_s ]						= 's';
		keycodes2char[ (int) asapi::keycodes::snapi_d ]						= 'd';
		keycodes2char[ (int) asapi::keycodes::snapi_f ]						= 'f';
		keycodes2char[ (int) asapi::keycodes::snapi_g ]						= 'g';
		keycodes2char[ (int) asapi::keycodes::snapi_h ]						= 'h';
		keycodes2char[ (int) asapi::keycodes::snapi_j ]						= 'j';
		keycodes2char[ (int) asapi::keycodes::snapi_k ]						= 'k';
		keycodes2char[ (int) asapi::keycodes::snapi_l ]						= 'l';
		keycodes2char[ (int) asapi::keycodes::snapi_semicolon ]				= ';';
		keycodes2char[ (int) asapi::keycodes::snapi_apostrophe ]				= '\'';
		keycodes2char[ (int) asapi::keycodes::snapi_grave ]					= '`';
		keycodes2char[ (int) asapi::keycodes::snapi_backslash ]				= '\\';
		keycodes2char[ (int) asapi::keycodes::snapi_z ]						= 'z';
		keycodes2char[ (int) asapi::keycodes::snapi_x ]						= 'x';
		keycodes2char[ (int) asapi::keycodes::snapi_c ]						= 'c';
		keycodes2char[ (int) asapi::keycodes::snapi_v ]						= 'v';
		keycodes2char[ (int) asapi::keycodes::snapi_b ]						= 'b';
		keycodes2char[ (int) asapi::keycodes::snapi_n ]						= 'n';
		keycodes2char[ (int) asapi::keycodes::snapi_m ]						= 'm';
		keycodes2char[ (int) asapi::keycodes::snapi_comma ]					= ',';
		keycodes2char[ (int) asapi::keycodes::snapi_dot ]						= '.';
		keycodes2char[ (int) asapi::keycodes::snapi_slash ]					= '/';
		keycodes2char[ (int) asapi::keycodes::snapi_kpasterisk ]				= '*';
		keycodes2char[ (int) asapi::keycodes::snapi_space ]					= ' ';
		keycodes2char[ (int) asapi::keycodes::snapi_kp7 ]						= '7';
		keycodes2char[ (int) asapi::keycodes::snapi_kp8 ]						= '8';
		keycodes2char[ (int) asapi::keycodes::snapi_kp9 ]						= '9';
		keycodes2char[ (int) asapi::keycodes::snapi_kpminus ]					= '-';
		keycodes2char[ (int) asapi::keycodes::snapi_kp4 ]						= '4';
		keycodes2char[ (int) asapi::keycodes::snapi_kp5 ]						= '5';
		keycodes2char[ (int) asapi::keycodes::snapi_kp6 ]						= '6';
		keycodes2char[ (int) asapi::keycodes::snapi_kpplus ]					= '+';
		keycodes2char[ (int) asapi::keycodes::snapi_kp1 ]						= '1';
		keycodes2char[ (int) asapi::keycodes::snapi_kp2 ]						= '2';
		keycodes2char[ (int) asapi::keycodes::snapi_kp3 ]						= '3';
		keycodes2char[ (int) asapi::keycodes::snapi_kp0 ]						= '0';
		keycodes2char[ (int) asapi::keycodes::snapi_kpdot ]					= ',';
	}
}

#endif