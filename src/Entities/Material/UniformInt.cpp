#include "UniformInt.hpp"
#include <GLES2/gl2.h>
#ifdef IS_EDITOR
#include "imgui.h"
#endif

namespace asapi
{

	void UniformInt::SetUniform(const int32_t& in)
	{
		m_data = in;
		m_isDirty = true;
	}

	void UniformInt::SendUniform()
	{
		if(m_isDirty)
		{
			glUniform1i(m_location, (int)m_data );
			m_isDirty = false;
		}
	}
	void UniformInt::SendUniform(const int32_t& override) const
	{
		glUniform1i(m_location, (int)override );
	}
	#ifdef IS_EDITOR
	void UniformInt::OnGUI()
	{
		ImGui::InputInt(m_name.c_str(), &m_data );
	}
	#endif

}