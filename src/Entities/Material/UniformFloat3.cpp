#include "UniformFloat3.hpp"
#include <GLES2/gl2.h>
#include "glm.hpp"
#include "ext.hpp"
#ifdef IS_EDITOR
#include "imgui.h"
#endif

namespace asapi
{
	void UniformFloat3::SetUniform(const glm::vec3& in)
	{
		m_data = in;
		m_isDirty = true;
	}

	void UniformFloat3::SendUniform()
	{
		if(m_isDirty)
		{
			glUniform3fv(m_location, 1, glm::value_ptr(m_data) );
			m_isDirty = false;
		}
	}
	void UniformFloat3::SendUniform(const glm::vec3& override) const
	{
		glUniform3fv(m_location, 1, glm::value_ptr(override) );
	}
	#ifdef IS_EDITOR
	void UniformFloat3::OnGUI()
	{
		ImGui::InputFloat3(m_name.c_str(), glm::value_ptr(m_data) );
	}
	#endif

}