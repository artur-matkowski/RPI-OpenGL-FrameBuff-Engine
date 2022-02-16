#include "SerializableObject.hpp"
#include "imgui.h"

namespace asapi
{
	void SerializableObjectBase::NextImGUIVecRow(const int row)
	{
        ImGui::TableNextRow();
	    ImGui::TableSetColumnIndex( 0 );
	    ImGui::Text("%d", row);
	    ImGui::TableSetColumnIndex( 1 );
	}
	void SerializableObjectBase::NextImGUIRow()
	{
        ImGui::TableNextRow();
	    ImGui::TableSetColumnIndex( 0 );
	}
}