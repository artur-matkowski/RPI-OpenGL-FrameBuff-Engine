#ifndef _H_ImguiXlib
#define _H_ImguiXlib
#ifndef IS_PLAYER
#include "imgui.h"      // IMGUI_IMPL_API

namespace asapgl
{
	IMGUI_IMPL_API bool     ImGui_ImplXlib_InitForOpenGL(long unsigned int window);
	IMGUI_IMPL_API void     ImGui_ImplXlib_Shutdown();
	IMGUI_IMPL_API void     ImGui_ImplXlib_NewFrame();
}

#endif
#endif