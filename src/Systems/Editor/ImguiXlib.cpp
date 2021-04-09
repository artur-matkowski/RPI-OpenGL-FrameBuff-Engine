#include "ImguiXlib.hpp"
#include "KeyCodes.hpp"
#include "Systems.hpp"
#include <ctime>
#include <ratio>
#ifdef USE_XLIB
#include <X11/Xlib.h>
#endif
#ifdef IS_EDITOR

namespace asapi
{
	static Xlib_EGL_ContextType::EGLWindow* g_Window = 0;
	static double               			g_Time = 0.0;
	static bool                 			g_MouseJustPressed[ImGuiMouseButton_COUNT] = {};
	static bool                 			g_MouseButtonState[ImGuiMouseButton_COUNT] = {};
	static bool                 			g_WantUpdateMonitors = true;
	static glm::ivec2 						g_MousePos;
	static glm::ivec2 						g_MousePosRoot;
	static Xlib_EGL_ContextType* 			g_Context = 0;


	static void ImGui_ImplXlib_InitPlatformInterface();
	static void ImGui_ImplXlib_UpdateMonitors();

	void MouseClickXlibCallback(void* receiver, void* data)
	{
		MouseClickEvent* args = (MouseClickEvent*)data;
		static ImGuiIO& io = ImGui::GetIO();

	    if (args->m_state == (int)asapi::keystates::snapi_down &&
	    	 (int)args->m_key >= 0 &&
	    	 (int)args->m_key < IM_ARRAYSIZE(g_MouseJustPressed))
	    {
	        g_MouseJustPressed[args->m_key] = true;
	    }
	    if( (int)args->m_key >= 0 &&
	    	 (int)args->m_key < IM_ARRAYSIZE(g_MouseJustPressed) )
	    {
	    	g_MouseButtonState[args->m_key] = args->m_state == (int)asapi::keystates::snapi_down;
	    }

	    if( args->m_key == (int)asapi::mousecodes::snapi_wheelY )
	    {
	    	if( args->m_state == (int)asapi::keystates::snapi_down )
		    	io.MouseWheel -= (float)1.0;
	    	else if( args->m_state == (int)asapi::keystates::snapi_up )
		    	io.MouseWheel += (float)1.0;
	    }
	    else if( args->m_key == (int)asapi::mousecodes::snapi_wheelX )
	    {
	    	if( args->m_state == (int)asapi::keystates::snapi_down )
		    	io.MouseWheelH -= (float)1.0;
	    	else if( args->m_state == (int)asapi::keystates::snapi_up )
		    	io.MouseWheelH += (float)1.0;
	    }
	}
	void KeyboardXlibCallback(void* receiver, void* data)
	{
		KeyboardEvent* args = (KeyboardEvent*)data;
		static ImGuiIO& io = ImGui::GetIO();

		if( args->m_char != '\0' && args->m_state == (int)asapi::keystates::snapi_down )
    		io.AddInputCharacter( args->m_char );

		if (args->m_state == (int)asapi::keystates::snapi_down)
	        io.KeysDown[args->m_key] = true;
	    if (args->m_state == (int)asapi::keystates::snapi_up)
	        io.KeysDown[args->m_key] = false;

	    // Modifiers are not reliable across systems
	    io.KeyCtrl = io.KeysDown[(int)asapi::keycodes::snapi_leftctrl] || io.KeysDown[(int)asapi::keycodes::snapi_rightctrl];
	    io.KeyShift = io.KeysDown[(int)asapi::keycodes::snapi_leftshift] || io.KeysDown[(int)asapi::keycodes::snapi_rightshift];
	    io.KeyAlt = io.KeysDown[(int)asapi::keycodes::snapi_leftalt] || io.KeysDown[(int)asapi::keycodes::snapi_rightalt];
	    //io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	}
	void MouseMoveXlibCallback(void* receiver, void* data)
	{
	    MouseMoveEvent* args = (MouseMoveEvent*)data;
	    g_MousePos.x = (int)args->m_Xpos;
	    g_MousePos.y = (int)args->m_Ypos;
	    g_MousePosRoot.x = (int)args->m_XposRoot;
	    g_MousePosRoot.y = (int)args->m_YposRoot;
	}


	int	error_handler(Display *display, XErrorEvent *err)
	{
		if(err->type == BadWindow)
		{
			log::error << "BadWindow" << std::endl;
		}
		else
		{
			log::error << "error_handler" << std::endl;
		}
	    return 0;
	}


	IMGUI_IMPL_API bool     ImGui_ImplXlib_InitForOpenGL(void* eglWindow, void* context)
	{
	    //XSetErrorHandler(error_handler);

	    g_Window = (Xlib_EGL_ContextType::EGLWindow*)eglWindow;
	    g_Context = (Xlib_EGL_ContextType*)context;
	    g_Time = 0.0;

	    // Setup backend capabilities flags
	    ImGuiIO& io = ImGui::GetIO();
	    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
	    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
	    io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;    // We can create multi-viewports on the Platform side (optional)
	#if GLFW_HAS_MOUSE_PASSTHROUGH || (GLFW_HAS_WINDOW_HOVERED && defined(_WIN32))
	    io.BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport; // We can set io.MouseHoveredViewport correctly (optional, not easy)
	#endif
	    io.BackendPlatformName = "imgui_impl_xlib";

	    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
	    io.KeyMap[ImGuiKey_Tab] 		= (int) asapi::keycodes::snapi_tab;
	    io.KeyMap[ImGuiKey_LeftArrow] 	= (int) asapi::keycodes::snapi_left;
	    io.KeyMap[ImGuiKey_RightArrow] 	= (int) asapi::keycodes::snapi_right;
	    io.KeyMap[ImGuiKey_UpArrow] 	= (int) asapi::keycodes::snapi_up;
	    io.KeyMap[ImGuiKey_DownArrow] 	= (int) asapi::keycodes::snapi_down;
	    io.KeyMap[ImGuiKey_PageUp] 		= (int) asapi::keycodes::snapi_pageup;
	    io.KeyMap[ImGuiKey_PageDown] 	= (int) asapi::keycodes::snapi_pagedown;
	    io.KeyMap[ImGuiKey_Home] 		= (int) asapi::keycodes::snapi_home;
	    io.KeyMap[ImGuiKey_End] 		= (int) asapi::keycodes::snapi_end;
	    io.KeyMap[ImGuiKey_Insert] 		= (int) asapi::keycodes::snapi_insert;
	    io.KeyMap[ImGuiKey_Delete] 		= (int) asapi::keycodes::snapi_delete;
	    io.KeyMap[ImGuiKey_Backspace]	= (int) asapi::keycodes::snapi_backspace;
	    io.KeyMap[ImGuiKey_Space] 		= (int) asapi::keycodes::snapi_space;
	    io.KeyMap[ImGuiKey_Enter] 		= (int) asapi::keycodes::snapi_enter;
	    io.KeyMap[ImGuiKey_Escape] 		= (int) asapi::keycodes::snapi_esc;
	    io.KeyMap[ImGuiKey_KeyPadEnter] = (int) asapi::keycodes::snapi_kpenter;
	    io.KeyMap[ImGuiKey_A] 			= (int) asapi::keycodes::snapi_a;
	    io.KeyMap[ImGuiKey_C] 			= (int) asapi::keycodes::snapi_c;
	    io.KeyMap[ImGuiKey_V] 			= (int) asapi::keycodes::snapi_v;
	    io.KeyMap[ImGuiKey_X] 			= (int) asapi::keycodes::snapi_x;
	    io.KeyMap[ImGuiKey_Y] 			= (int) asapi::keycodes::snapi_y;
	    io.KeyMap[ImGuiKey_Z] 			= (int) asapi::keycodes::snapi_z;

	    //io.SetClipboardTextFn = ImGui_ImplXlib_SetClipboardText;
	    //io.GetClipboardTextFn = ImGui_ImplXlib_GetClipboardText;
	    //io.ClipboardUserData = g_Window;

		bfu::EventSystem& es = SYSTEMS::GetObject().EVENTS;

		es.GetFastEvent("MouseClickEvent")->RegisterCallback(nullptr, MouseClickXlibCallback);
		es.GetFastEvent("KeyboardEvent")->RegisterCallback(nullptr, KeyboardXlibCallback);
		es.GetFastEvent("MouseMoveEvent")->RegisterCallback(nullptr, MouseMoveXlibCallback);


   // Update monitors the first time (note: monitor callback are broken in GLFW 3.2 and earlier, see github.com/glfw/glfw/issues/784)
	    ImGui_ImplXlib_UpdateMonitors();

	    // Our mouse update function expect PlatformHandle to be filled for the main viewport
	    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	    main_viewport->PlatformHandle = (void*)g_Window;

	    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	        ImGui_ImplXlib_InitPlatformInterface();

	    return true;
	}
	
	static void ImGui_ImplXlib_CreateWindow(ImGuiViewport* viewport)
	{
		
	    Xlib_EGL_ContextType::EGLWindow* data = g_Context->window_create( "No Title Yet", 
	    															(int)viewport->Pos.x, (int)viewport->Pos.y,
	    															(int)viewport->Size.x, (int)viewport->Size.y);

	    viewport->PlatformUserData = data;
	    data->WindowOwned = true;
	    viewport->PlatformHandle = (void*)data;
	}

	static void ImGui_ImplXlib_DestroyWindow(ImGuiViewport* viewport)
	{
		log::debug << "ImGui_ImplXlib_DestroyWindow" << std::endl;
	    Xlib_EGL_ContextType::EGLWindow* data = (Xlib_EGL_ContextType::EGLWindow*)viewport->PlatformUserData;


		eglDestroySurface(g_Context->GetEGLDisplay(), data->surface);
		//eglDestroyContext(g_Context->GetEGLDisplay(), data->context);
		//XUnmapWindow(g_Context->GetDisplay(), data->x11);
		XDestroyWindow(g_Context->GetDisplay(), data->x11);

		g_Context->Erase(data);

    	viewport->PlatformUserData = viewport->PlatformHandle = NULL;
		
	}

	static void ImGui_ImplXlib_ShowWindow(ImGuiViewport* viewport)
	{
	    Xlib_EGL_ContextType::EGLWindow* data = (Xlib_EGL_ContextType::EGLWindow*)viewport->PlatformUserData;

	    g_Context->window_show( *data );
	}

	static ImVec2 ImGui_ImplXlib_GetWindowPos(ImGuiViewport* viewport)
	{
		Xlib_EGL_ContextType::EGLWindow* data = (Xlib_EGL_ContextType::EGLWindow*) viewport->PlatformUserData;

	    return ImVec2((float)data->position.x, (float)data->position.y);
	}

	static void ImGui_ImplXlib_SetWindowPos(ImGuiViewport* viewport, ImVec2 pos)
	{
		Xlib_EGL_ContextType::EGLWindow* data = (Xlib_EGL_ContextType::EGLWindow*) viewport->PlatformUserData;
		XWindowChanges setup;
		setup.x = pos.x;
		setup.y = pos.y;

		XConfigureWindow( g_Context->GetDisplay(), data->x11, CWX | CWY, &setup);
		//log::debug << "ImGui_ImplXlib_SetWindowPos" << std::endl;
	}

	static ImVec2 ImGui_ImplXlib_GetWindowSize(ImGuiViewport* viewport)
	{
		log::debug << " TBD ImGui_ImplXlib_GetWindowSize" << std::endl;
		return ImVec2(0, 0);
	}

	static void ImGui_ImplXlib_SetWindowSize(ImGuiViewport* viewport, ImVec2 size)
	{
		Xlib_EGL_ContextType::EGLWindow* data = (Xlib_EGL_ContextType::EGLWindow*) viewport->PlatformUserData;
		XWindowChanges setup;
		setup.width = size.x;
		setup.height = size.y;

		XConfigureWindow( g_Context->GetDisplay(), data->x11, CWWidth | CWHeight, &setup);

		//log::debug << "ImGui_ImplXlib_SetWindowSize" << std::endl;
	}

	static void ImGui_ImplXlib_SetWindowTitle(ImGuiViewport* viewport, const char* title)
	{
		Xlib_EGL_ContextType::EGLWindow* data = (Xlib_EGL_ContextType::EGLWindow*) viewport->PlatformUserData;

		XStoreName( g_Context->GetDisplay(), data->x11, title );
		//log::debug << "ImGui_ImplXlib_SetWindowTitle" << std::endl;
	}

	static void ImGui_ImplXlib_SetWindowFocus(ImGuiViewport* viewport)
	{
		log::debug << "ImGui_ImplXlib_SetWindowFocus" << std::endl;
	}

	static bool ImGui_ImplXlib_GetWindowFocus(ImGuiViewport* viewport)
	{
		Xlib_EGL_ContextType::EGLWindow* data = (Xlib_EGL_ContextType::EGLWindow*) viewport->PlatformUserData;
		// log::debug << "ImGui_ImplXlib_GetWindowFocus" << g_Context->GetFocusedWindow() << std::endl;
		// log::debug << "ImGui_ImplXlib_GetWindowFocus" << data->x11 << std::endl;
		// log::debug << " " << std::endl;
		return g_Context->GetFocusedWindow() == data->x11;
	}

	static bool ImGui_ImplXlib_GetWindowMinimized(ImGuiViewport* viewport)
	{
		//log::debug << "ImGui_ImplXlib_GetWindowMinimized" << std::endl;
		return false;
	}

	static void ImGui_ImplXlib_RenderWindow(ImGuiViewport* viewport, void*)
	{
		Xlib_EGL_ContextType::EGLWindow* data = (Xlib_EGL_ContextType::EGLWindow*) viewport->PlatformUserData;

		//eglMakeCurrent(g_Context->GetEGLDisplay(), data->surface, data->surface, data->context);
		eglMakeCurrent( eglGetCurrentDisplay(), data->surface, data->surface, eglGetCurrentContext() );
	}

	static void ImGui_ImplXlib_SwapBuffers(ImGuiViewport* viewport, void*)
	{
		Xlib_EGL_ContextType::EGLWindow* data = (Xlib_EGL_ContextType::EGLWindow*) viewport->PlatformUserData;

		//eglMakeCurrent(g_Context->GetEGLDisplay(), data->surface, data->surface, data->context);
		eglMakeCurrent( eglGetCurrentDisplay(), data->surface, data->surface, eglGetCurrentContext() );
		eglSwapBuffers(g_Context->GetEGLDisplay(), data->surface);
	}


	static void ImGui_ImplXlib_UpdateMonitors()
	{
		Display *display = g_Context->GetDisplay();
		Screen *screen;
	    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
	    platform_io.Monitors.resize(0);

		// return the number of available screens
		int count_screens = ScreenCount(display);

		for (int i = 0; i < count_screens; ++i) {
	        ImGuiPlatformMonitor monitor;
		    screen = ScreenOfDisplay(display, i);
	        monitor.MainPos = monitor.WorkPos = ImVec2((float)0.0f, (float)0.0f);
	        monitor.MainSize = monitor.WorkSize = ImVec2((float)screen->width, (float)screen->height);

	        platform_io.Monitors.push_back(monitor);
		}

	    g_WantUpdateMonitors = false;
	}



	static void ImGui_ImplXlib_InitPlatformInterface()
	{
	    // Register platform interface (will be coupled with a renderer interface)
	    ImGuiPlatformIO& platform_io 				= ImGui::GetPlatformIO();
	    
	    platform_io.Platform_CreateWindow 			= ImGui_ImplXlib_CreateWindow;
	    platform_io.Platform_DestroyWindow 			= ImGui_ImplXlib_DestroyWindow;
	    platform_io.Platform_ShowWindow 			= ImGui_ImplXlib_ShowWindow;
	    platform_io.Platform_SetWindowPos 			= ImGui_ImplXlib_SetWindowPos;
	    platform_io.Platform_GetWindowPos 			= ImGui_ImplXlib_GetWindowPos;
	    platform_io.Platform_SetWindowSize 			= ImGui_ImplXlib_SetWindowSize;
	    platform_io.Platform_GetWindowSize 			= ImGui_ImplXlib_GetWindowSize;
	    platform_io.Platform_SetWindowFocus 		= ImGui_ImplXlib_SetWindowFocus;
	    platform_io.Platform_GetWindowFocus 		= ImGui_ImplXlib_GetWindowFocus;
	    platform_io.Platform_GetWindowMinimized 	= ImGui_ImplXlib_GetWindowMinimized;
	    platform_io.Platform_SetWindowTitle 		= ImGui_ImplXlib_SetWindowTitle;
	    platform_io.Platform_RenderWindow 			= ImGui_ImplXlib_RenderWindow;
	    platform_io.Platform_SwapBuffers 			= ImGui_ImplXlib_SwapBuffers;
	

	    // Register main window handle (which is owned by the main application, not by us)
	    // This is mostly for simplicity and consistency, so that our code (e.g. mouse handling etc.) can use same logic for main and secondary viewports.
	    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	    Xlib_EGL_ContextType::EGLWindow* data = g_Context->GetMainEGLWindow();
	    data->WindowOwned = false;
	    main_viewport->PlatformUserData = data;
	    main_viewport->PlatformHandle = (void*)g_Window;
	}

	static void ImGui_ImplXlib_UpdateMousePosAndButtons()
	{
		
	    // Update buttons
	    ImGuiIO& io = ImGui::GetIO();
	    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
	    {
	        // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
	        io.MouseDown[i] = g_MouseJustPressed[i] || g_MouseButtonState[i]!=false;
	        g_MouseJustPressed[i] = false;
	    }

	    // Update mouse position
	    const ImVec2 mouse_pos_backup = io.MousePos;
	    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
	    io.MouseHoveredViewport = 0;
	    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
	    for (int n = 0; n < platform_io.Viewports.Size; n++)
	    {
	        ImGuiViewport* viewport = platform_io.Viewports[n];
	        Xlib_EGL_ContextType::EGLWindow* window = (Xlib_EGL_ContextType::EGLWindow*)viewport->PlatformHandle;
	        IM_ASSERT(window != NULL);

	        const bool focused = g_Context->GetFocusedWindow() == window->x11;

	        if (focused)
	        {
	            if (io.WantSetMousePos)
	            {
	            	XWarpPointer(g_Context->GetDisplay(), None, g_Context->GetRoot(), 0, 0, 0, 0, (int)(mouse_pos_backup.x - viewport->Pos.x), (int)(mouse_pos_backup.y - viewport->Pos.y) );
	            }
	            else
	            {
	                if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	                {
	                    // Multi-viewport mode: mouse position in OS absolute coordinates (io.MousePos is (0,0) when the mouse is on the upper-left of the primary monitor)
	                    io.MousePos = ImVec2((float)g_MousePosRoot.x, (float)g_MousePosRoot.y);
	                }
	                else
	                {
	                    // Single viewport mode: mouse position in client window coordinates (io.MousePos is (0,0) when the mouse is on the upper-left corner of the app window)
	                    io.MousePos = ImVec2((float)window->cursorPos.x, (float)window->cursorPos.y);
	                }
	            }
	             for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
	                 io.MouseDown[i] |= g_MouseButtonState[i]!=false;
	        }


	    }
	}

	void ImGui_ImplXlib_NewFrame()
	{
	    ImGuiIO& io = ImGui::GetIO();
	    IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer backend. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

	    // Setup display size (every frame to accommodate for window resizing)
	    int w, h;
	    int display_w, display_h;
	    display_w = w = g_Window->resolution.x;
	    display_h = h = g_Window->resolution.y;
	    io.DisplaySize = ImVec2((float)w, (float)h);
	    if (w > 0 && h > 0)
	        io.DisplayFramebufferScale = ImVec2((float)display_w / w, (float)display_h / h);
	    if (g_WantUpdateMonitors)
	        ImGui_ImplXlib_UpdateMonitors();
	   
	    ImGui_ImplXlib_UpdateMousePosAndButtons();
	    //ImGui_ImplXlib_UpdateMouseCursor();

	    // Update game controllers (if enabled and available)
	    //ImGui_ImplXlib_UpdateGamepads();
	}

	void ImGui_ImplXlib_Shutdown()
	{
	    // Cleanup
	    ImGui_ImplOpenGL3_Shutdown();
	    ImGui::DestroyContext();
	}

	static const char* ImGui_ImplXlib_GetClipboardText(void* user_data)
	{
	    return "TBD https://github.com/exebook/x11clipboard/blob/master/x11copy.c";
	}

	static void ImGui_ImplXlib_SetClipboardText(void* user_data, const char* text)
	{
	    //https://github.com/exebook/x11clipboard/blob/master/x11paste.c
	}

}

#endif