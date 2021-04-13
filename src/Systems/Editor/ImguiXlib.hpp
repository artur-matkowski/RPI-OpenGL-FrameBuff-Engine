#ifndef _H_ImguiXlib
#define _H_ImguiXlib
#include "object.hpp"
#ifdef IS_EDITOR
#include "imgui.h"      // IMGUI_IMPL_API
#include "backends/imgui_impl_opengl3.h"


namespace asapi
{
	IMGUI_IMPL_API bool     ImGui_ImplXlib_InitForOpenGL(void* eglWindow, void* context);
	IMGUI_IMPL_API void     ImGui_ImplXlib_NewFrame();
	IMGUI_IMPL_API void     ImGui_ImplXlib_Shutdown();
}

#endif
#endif