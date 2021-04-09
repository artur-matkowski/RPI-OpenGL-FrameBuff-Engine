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
	public:
		EditorSystem();
		~EditorSystem();

		void Init();

		void OnGUI();
		
	};
}
#endif
#endif