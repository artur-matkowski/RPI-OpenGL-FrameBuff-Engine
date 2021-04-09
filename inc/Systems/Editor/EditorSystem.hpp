#ifndef _H_EditorSystem
#define _H_EditorSystem
#ifdef IS_EDITOR
#include "glm.hpp"
#include "imgui.h"      // IMGUI_IMPL_API
#include "Mesh.hpp"
#include "MaterialType.hpp"
#include "UniformBase.hpp"

namespace asapi
{
	class EditorSystem
	{
		ImGuiIO* io = nullptr;
	public:
		EditorSystem();
		~EditorSystem();

		void Init();

		void OnGUI();

		inline void SetDeltaTime(float dt){ io->DeltaTime = dt; }
		
	};
}
#endif
#endif