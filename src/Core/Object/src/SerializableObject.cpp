#include "SerializableObject.hpp"
#include "imgui.h"

namespace asapi
{
	void SerializableObjectBase::NextImGUIRow()
	{
        ImGui::TableNextRow();
	    ImGui::TableSetColumnIndex(0);
	}
}