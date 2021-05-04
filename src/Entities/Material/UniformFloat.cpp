#include "UniformFloat.hpp"
#include <GLES2/gl2.h>
#ifdef IS_EDITOR
#include "imgui.h"
#endif

namespace asapi
{
	void UniformFloat::SetUniform(const float& in)
	{
		m_data = in;
		m_isDirty = true;
	}

	void UniformFloat::SendUniform()
	{
		if(m_isDirty)
		{
			glUniform1f(m_location, (float)m_data );
			m_isDirty = false;
		}
	}
	void UniformFloat::SendUniform(const float& override) const
	{
		glUniform1f(m_location, override );
	}
	#ifdef IS_EDITOR
	void UniformFloat::OnGUI()
	{
		ImGui::InputFloat(m_name.c_str(), &m_data );
	}
	#endif

}