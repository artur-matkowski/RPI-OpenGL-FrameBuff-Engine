#include "UniformBase.hpp"
#include "MaterialType.hpp"
#include "Systems.hpp"
#include <GLES2/gl2.h>
#include <gtc/type_ptr.hpp>
#ifdef IS_EDITOR
#include "imgui.h"
#endif

namespace asapi
{

	UniformInterface::~UniformInterface()
	{}


	#ifdef IS_EDITOR
	void UniformInterface::OnGUI()
	{
		ImGui::LabelText("Unsuported Uniform %s", "---");
	}
	#endif

}