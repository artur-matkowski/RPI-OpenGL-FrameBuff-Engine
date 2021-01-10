#ifndef _H_ImguiDRM
#define _H_ImguiDRM
#include "object.hpp"
#ifdef IS_EDITOR
#include "imgui.h"      // IMGUI_IMPL_API
#include "backends/imgui_impl_opengl3.h"


namespace asapgl
{
	IMGUI_IMPL_API bool     ImGui_ImplDRM_InitForOpenGL(void* eglWindow, void* context);
	IMGUI_IMPL_API void     ImGui_ImplDRM_NewFrame();
	IMGUI_IMPL_API void     ImGui_ImplDRM_Shutdown();
}

#endif
#endif