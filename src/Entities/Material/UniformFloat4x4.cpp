#include "UniformFloat4x4.hpp"
#include <GLES2/gl2.h>
#include "glm.hpp"
#include "ext.hpp"
#ifdef IS_EDITOR
#include "imgui.h"
#endif

namespace asapi
{
	void UniformFloat4x4::SetUniform(const glm::mat4& in)
	{
		m_data = in;
		m_isDirty = true;
	}


	void UniformFloat4x4::SendUniform()
	{
		if(m_isDirty)
		{
			glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(m_data) );
			m_isDirty = false;
		}
	}
	void UniformFloat4x4::SendUniform(const glm::mat4& override) const
	{
		glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(override) );
	}
	#ifdef IS_EDITOR
	void UniformFloat4x4::OnGUI()
	{
		if (ImGui::TreeNode("Used model matrix"))
		{
			const float* data = glm::value_ptr(m_data);


			ImGui::LabelText(m_name.c_str(), "%.3f    %.3f    %.3f    %.3f"		
															, data[0]
															, data[4] 
															, data[8] 
															, data[12] );
			ImGui::Text("%.3f    %.3f    %.3f    %.3f"		, data[1]
															, data[5] 
															, data[9] 
															, data[13] );
			ImGui::Text("%.3f    %.3f    %.3f    %.3f"		, data[2]
															, data[6] 
															, data[10] 
															, data[14] );
			ImGui::Text("%.3f    %.3f    %.3f    %.3f"		, data[3]
															, data[7] 
															, data[11] 
															, data[15] );

	        ImGui::TreePop();
	    }
	}
	#endif

}