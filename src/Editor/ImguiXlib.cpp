#include "ImguiXlib.hpp"
#include "KeyCodes.hpp"


namespace asapgl
{
	static long unsigned int 	g_Window = 0;
	static double               g_Time = 0.0;


	IMGUI_IMPL_API bool     ImGui_ImplXlib_InitForOpenGL(long unsigned int window)
	{
	    g_Window = window;
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
/*
	    // Create mouse cursors
	    // (By design, on X11 cursors are user configurable and some cursors may be missing. When a cursor doesn't exist,
	    // GLFW will emit an error which will often be printed by the app, so we temporarily disable error reporting.
	    // Missing cursors will return NULL and our _UpdateMouseCursor() function will use the Arrow cursor instead.)
	    GLFWerrorfun prev_error_callback = glfwSetErrorCallback(NULL);
	    g_MouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	    g_MouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
	    g_MouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
	    g_MouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
	    g_MouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
	#if GLFW_HAS_NEW_CURSORS
	    g_MouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
	    g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
	    g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
	    g_MouseCursors[ImGuiMouseCursor_NotAllowed] = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
	#else
	    g_MouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	    g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	    g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	    g_MouseCursors[ImGuiMouseCursor_NotAllowed] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	#endif
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
	    glfwSetMonitorCallback(ImGui_ImplGlfw_MonitorCallback);

	    // Our mouse update function expect PlatformHandle to be filled for the main viewport
	    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	    main_viewport->PlatformHandle = (void*)g_Window;
	#ifdef _WIN32
	    main_viewport->PlatformHandleRaw = glfwGetWin32Window(g_Window);
	#endif
	    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	        ImGui_ImplGlfw_InitPlatformInterface();

	    g_ClientApi = client_api;*/
	    return true;
	}
}