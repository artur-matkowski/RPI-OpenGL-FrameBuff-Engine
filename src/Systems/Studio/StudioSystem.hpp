#ifndef _H_EditorSystem
#define _H_EditorSystem
#ifdef IS_EDITOR
#include "glm.hpp"
#include "Mesh_old.hpp"
#include "MaterialType.hpp"
#include "UniformBase.hpp"

namespace asapi
{
	class StudioSystem
	{
	public:
		StudioSystem();
		~StudioSystem();

		void Init(const int argc, const char** argv);

		void OnGUI();
		
	};
}
#endif
#endif