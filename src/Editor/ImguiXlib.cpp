#include "ImguiXlib.hpp"
#include "KeyCodes.hpp"
#include "Systems.hpp"

#ifdef IS_EDITOR

namespace asapgl
{
	static Xlib_EGL_ContextType::EGLWindow* g_Window = 0;
	static double               			g_Time = 0.0;
	static bool                 			g_MouseJustPressed[ImGuiMouseButton_COUNT] = {};
	static glm::ivec2 						g_MousePos;


	static void ImGui_ImplXlib_InitPlatformInterface();



	IMGUI_IMPL_API bool     ImGui_ImplXlib_InitForOpenGL(void* eglWindow)
	{
	    g_Window = (Xlib_EGL_ContextType::EGLWindow*)eglWindow;
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
	    io.KeyMap[ImGuiKey_Tab] 		= (int) asapgl::keycodes::snapi_tab;
	    io.KeyMap[ImGuiKey_LeftArrow] 	= (int) asapgl::keycodes::snapi_left;
	    io.KeyMap[ImGuiKey_RightArrow] 	= (int) asapgl::keycodes::snapi_right;
	    io.KeyMap[ImGuiKey_UpArrow] 	= (int) asapgl::keycodes::snapi_up;
	    io.KeyMap[ImGuiKey_DownArrow] 	= (int) asapgl::keycodes::snapi_down;
	    io.KeyMap[ImGuiKey_PageUp] 		= (int) asapgl::keycodes::snapi_pageup;
	    io.KeyMap[ImGuiKey_PageDown] 	= (int) asapgl::keycodes::snapi_pagedown;
	    io.KeyMap[ImGuiKey_Home] 		= (int) asapgl::keycodes::snapi_home;
	    io.KeyMap[ImGuiKey_End] 		= (int) asapgl::keycodes::snapi_end;
	    io.KeyMap[ImGuiKey_Insert] 		= (int) asapgl::keycodes::snapi_insert;
	    io.KeyMap[ImGuiKey_Delete] 		= (int) asapgl::keycodes::snapi_delete;
	    io.KeyMap[ImGuiKey_Backspace]	= (int) asapgl::keycodes::snapi_backspace;
	    io.KeyMap[ImGuiKey_Space] 		= (int) asapgl::keycodes::snapi_space;
	    io.KeyMap[ImGuiKey_Enter] 		= (int) asapgl::keycodes::snapi_enter;
	    io.KeyMap[ImGuiKey_Escape] 		= (int) asapgl::keycodes::snapi_esc;
	    io.KeyMap[ImGuiKey_KeyPadEnter] = (int) asapgl::keycodes::snapi_kpenter;
	    io.KeyMap[ImGuiKey_A] 			= (int) asapgl::keycodes::snapi_a;
	    io.KeyMap[ImGuiKey_C] 			= (int) asapgl::keycodes::snapi_c;
	    io.KeyMap[ImGuiKey_V] 			= (int) asapgl::keycodes::snapi_v;
	    io.KeyMap[ImGuiKey_X] 			= (int) asapgl::keycodes::snapi_x;
	    io.KeyMap[ImGuiKey_Y] 			= (int) asapgl::keycodes::snapi_y;
	    io.KeyMap[ImGuiKey_Z] 			= (int) asapgl::keycodes::snapi_z;

	    //io.SetClipboardTextFn = ImGui_ImplGlfw_SetClipboardText;
	    //io.GetClipboardTextFn = ImGui_ImplGlfw_GetClipboardText;
	    //io.ClipboardUserData = g_Window;

		bfu::CallbackId id;
		bfu::EventSystem& events = SYSTEMS::GetObject().EVENTS;

	    events.RegisterCallback<MouseClickEvent>(id, [&](bfu::EventArgsBase& a)
	    {
		    MouseClickEvent* args = (MouseClickEvent*)&a;
			static ImGuiIO& io = ImGui::GetIO();

		    if (args->m_state == (int)asapgl::keystates::snapi_down
		    	&& (int)args->m_key >= 0 
		    	&& (int)args->m_key < IM_ARRAYSIZE(g_MouseJustPressed))
		    {
		        g_MouseJustPressed[args->m_key] = true;
		    }
		    else if( args->m_key == (int)asapgl::mousecodes::snapi_wheelY )
		    {
		    	if( args->m_state == (int)asapgl::keystates::snapi_down )
			    	io.MouseWheel -= (float)1.0;
		    	else if( args->m_state == (int)asapgl::keystates::snapi_up )
			    	io.MouseWheel += (float)1.0;
		    }
		    else if( args->m_key == (int)asapgl::mousecodes::snapi_wheelX )
		    {
		    	if( args->m_state == (int)asapgl::keystates::snapi_down )
			    	io.MouseWheelH -= (float)1.0;
		    	else if( args->m_state == (int)asapgl::keystates::snapi_up )
			    	io.MouseWheelH += (float)1.0;
		    }
	    });

	    events.RegisterCallback<KeyboardEvent>(id, [&](bfu::EventArgsBase& a)
	    {
		    KeyboardEvent* args = (KeyboardEvent*)&a;
			static ImGuiIO& io = ImGui::GetIO();

			if( args->m_char != '\0' && args->m_state == (int)asapgl::keystates::snapi_down )
	    		io.AddInputCharacter( args->m_char );

			if (args->m_state == (int)asapgl::keystates::snapi_down)
		        io.KeysDown[args->m_key] = true;
		    if (args->m_state == (int)asapgl::keystates::snapi_up)
		        io.KeysDown[args->m_key] = false;

		    // Modifiers are not reliable across systems
		    io.KeyCtrl = io.KeysDown[(int)asapgl::keycodes::snapi_leftctrl] || io.KeysDown[(int)asapgl::keycodes::snapi_rightctrl];
		    io.KeyShift = io.KeysDown[(int)asapgl::keycodes::snapi_leftshift] || io.KeysDown[(int)asapgl::keycodes::snapi_rightshift];
		    io.KeyAlt = io.KeysDown[(int)asapgl::keycodes::snapi_leftalt] || io.KeysDown[(int)asapgl::keycodes::snapi_rightalt];
		    //io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	    });

	    events.RegisterCallback<MouseMoveEvent>(id, [&](bfu::EventArgsBase& a)
	    {
		    MouseMoveEvent* args = (MouseMoveEvent*)&a;
		    g_MousePos.x = (int)args->m_Xpos;
		    g_MousePos.y = (int)args->m_Ypos;
	    });

/*
	    // Create mouse cursors
	    // (By design, on X11 cursors are user configurable and some cursors may be missing. When a cursor doesn't exist,
	    // GLFW will emit an error which will often be printed by the app, so we temporarily disable error reporting.
	    // Missing cursors will return NULL and our _UpdateMouseCursor() function will use the Arrow cursor instead.)
	    GLFWerrorfun prev_error_callback = glfwSetErrorCallback(NULL);

	    glfwSetErrorCallback(prev_error_callback);

	    // Chain GLFW callbacks: our callbacks will call the user's previously installed callbacks, if any.
	    g_PrevUserCallbackMousebutton = NULL;
	    g_PrevUserCallbackScroll = NULL;
	    g_PrevUserCallbackKey = NULL;
	    g_PrevUserCallbackChar = NULL;
	    g_PrevUserCallbackMonitor = NULL;
	    if (install_callbacks)
	    {
	        g_InstalledCallbacks = true;
	        g_PrevUserCallbackMousebutton = glfwSetMouseButtonCallback(window, ImGui_ImplGlfw_MouseButtonCallback);
	        g_PrevUserCallbackScroll = glfwSetScrollCallback(window, ImGui_ImplGlfw_ScrollCallback);
	        g_PrevUserCallbackKey = glfwSetKeyCallback(window, ImGui_ImplGlfw_KeyCallback);
	        g_PrevUserCallbackChar = glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
	        g_PrevUserCallbackMonitor = glfwSetMonitorCallback(ImGui_ImplGlfw_MonitorCallback);
	    }

	    // Update monitors the first time (note: monitor callback are broken in GLFW 3.2 and earlier, see github.com/glfw/glfw/issues/784)
	    ImGui_ImplGlfw_UpdateMonitors();
	    glfwSetMonitorCallback(ImGui_ImplGlfw_MonitorCallback);*/

	    // Our mouse update function expect PlatformHandle to be filled for the main viewport
	    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	    main_viewport->PlatformHandle = (void*)g_Window;

	    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	        ImGui_ImplXlib_InitPlatformInterface();

	    return true;
	}

	static void ImGui_ImplXlib_InitPlatformInterface()
	{
	    // Register platform interface (will be coupled with a renderer interface)
	    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();/*
	    platform_io.Platform_CreateWindow = ImGui_ImplGlfw_CreateWindow;
	    platform_io.Platform_DestroyWindow = ImGui_ImplGlfw_DestroyWindow;
	    platform_io.Platform_ShowWindow = ImGui_ImplGlfw_ShowWindow;
	    platform_io.Platform_SetWindowPos = ImGui_ImplGlfw_SetWindowPos;
	    platform_io.Platform_GetWindowPos = ImGui_ImplGlfw_GetWindowPos;
	    platform_io.Platform_SetWindowSize = ImGui_ImplGlfw_SetWindowSize;
	    platform_io.Platform_GetWindowSize = ImGui_ImplGlfw_GetWindowSize;
	    platform_io.Platform_SetWindowFocus = ImGui_ImplGlfw_SetWindowFocus;
	    platform_io.Platform_GetWindowFocus = ImGui_ImplGlfw_GetWindowFocus;
	    platform_io.Platform_GetWindowMinimized = ImGui_ImplGlfw_GetWindowMinimized;
	    platform_io.Platform_SetWindowTitle = ImGui_ImplGlfw_SetWindowTitle;
	    platform_io.Platform_RenderWindow = ImGui_ImplGlfw_RenderWindow;
	    platform_io.Platform_SwapBuffers = ImGui_ImplGlfw_SwapBuffers;*/
	
/*
	    // Register main window handle (which is owned by the main application, not by us)
	    // This is mostly for simplicity and consistency, so that our code (e.g. mouse handling etc.) can use same logic for main and secondary viewports.
	    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	    ImGuiViewportDataGlfw* data = IM_NEW(ImGuiViewportDataGlfw)();
	    data->Window = g_Window;
	    data->WindowOwned = false;
	    main_viewport->PlatformUserData = data;
	    main_viewport->PlatformHandle = (void*)g_Window;*/
	}

	static void ImGui_ImplXlib_UpdateMousePosAndButtons()
	{
		/*
	    // Update buttons
	    ImGuiIO& io = ImGui::GetIO();
	    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
	    {
	        // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
	        io.MouseDown[i] = g_MouseJustPressed[i] || glfwGetMouseButton(g_Window, i) != 0;
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
	        GLFWwindow* window = (GLFWwindow*)viewport->PlatformHandle;
	        IM_ASSERT(window != NULL);
	#ifdef __EMSCRIPTEN__
	        const bool focused = true;
	        IM_ASSERT(platform_io.Viewports.Size == 1);
	#else
	        const bool focused = glfwGetWindowAttrib(window, GLFW_FOCUSED) != 0;
	#endif
	        if (focused)
	        {
	            if (io.WantSetMousePos)
	            {
	                glfwSetCursorPos(window, (double)(mouse_pos_backup.x - viewport->Pos.x), (double)(mouse_pos_backup.y - viewport->Pos.y));
	            }
	            else
	            {
	                double mouse_x, mouse_y;
	                glfwGetCursorPos(window, &mouse_x, &mouse_y);
	                if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	                {
	                    // Multi-viewport mode: mouse position in OS absolute coordinates (io.MousePos is (0,0) when the mouse is on the upper-left of the primary monitor)
	                    int window_x, window_y;
	                    glfwGetWindowPos(window, &window_x, &window_y);
	                    io.MousePos = ImVec2((float)mouse_x + window_x, (float)mouse_y + window_y);
	                }
	                else
	                {
	                    // Single viewport mode: mouse position in client window coordinates (io.MousePos is (0,0) when the mouse is on the upper-left corner of the app window)
	                    io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);
	                }
	            }
	            for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
	                io.MouseDown[i] |= glfwGetMouseButton(window, i) != 0;
	        }

	        // (Optional) When using multiple viewports: set io.MouseHoveredViewport to the viewport the OS mouse cursor is hovering.
	        // Important: this information is not easy to provide and many high-level windowing library won't be able to provide it correctly, because
	        // - This is _ignoring_ viewports with the ImGuiViewportFlags_NoInputs flag (pass-through windows).
	        // - This is _regardless_ of whether another viewport is focused or being dragged from.
	        // If ImGuiBackendFlags_HasMouseHoveredViewport is not set by the backend, imgui will ignore this field and infer the information by relying on the
	        // rectangles and last focused time of every viewports it knows about. It will be unaware of other windows that may be sitting between or over your windows.
	        // [GLFW] FIXME: This is currently only correct on Win32. See what we do below with the WM_NCHITTEST, missing an equivalent for other systems.
	        // See https://github.com/glfw/glfw/issues/1236 if you want to help in making this a GLFW feature.
	#if GLFW_HAS_MOUSE_PASSTHROUGH || (GLFW_HAS_WINDOW_HOVERED && defined(_WIN32))
	        const bool window_no_input = (viewport->Flags & ImGuiViewportFlags_NoInputs) != 0;
	#if GLFW_HAS_MOUSE_PASSTHROUGH
	        glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, window_no_input);
	#endif
	        if (glfwGetWindowAttrib(window, GLFW_HOVERED) && !window_no_input)
	            io.MouseHoveredViewport = viewport->ID;
	#endif
	    }*/
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
	   
	    //if (g_WantUpdateMonitors)
	    //    ImGui_ImplGlfw_UpdateMonitors();

	    // Setup time step
	    double current_time = SYSTEMS::GetObject().TIME.TimeSinceApplicationStart();
	    io.DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f / 60.0f);
	    g_Time = current_time;

	    ImGui_ImplXlib_UpdateMousePosAndButtons();
	    //ImGui_ImplGlfw_UpdateMouseCursor();

	    // Update game controllers (if enabled and available)
	    //ImGui_ImplGlfw_UpdateGamepads();
	}

	void ImGui_ImplXlib_Shutdown()
	{
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