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

	UniformBase::~UniformBase()
	{}


	#ifdef IS_EDITOR
	void UniformBase::OnGUI()
	{
		ImGui::LabelText("Unsuported Uniform %s", "---");
	}
	#endif

}