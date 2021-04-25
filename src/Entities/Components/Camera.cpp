#include "Camera.hpp"
#include "ext.hpp"
#include "Systems.hpp"
#ifdef IS_EDITOR
#include "imgui.h"
#endif

namespace asapi
{
	Camera::Camera(bfu::MemBlockBase* mBlock)
		:ComponentBase<Camera>(mBlock)
	{
		m_nearPlane = 0.1f;
		m_farPlane = 100.0f;
		m_FOV = 45.0f;
	};

	void Camera::OnIsDirty()
	{
		glm::ivec2 res;
		SYSTEMS::GetObject().RENDERER.GetResolution(res);
		m_projectionMatrix = glm::perspective(glm::radians(m_FOV), ((float)res.x)/res.y, m_nearPlane, m_farPlane);

		if(m_isMainCamera)
			SYSTEMS::GetObject().RENDERER.UpdateProjectionMatrix(m_projectionMatrix);
	}

	#ifdef IS_EDITOR
	void Camera::OnGUI()
	{
		const auto flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll;
		
		bool updated = false;
		updated |= ImGui::InputFloat("Near Plane", &m_nearPlane, flags);
		updated |= ImGui::InputFloat("Far Plane", &m_farPlane, flags);
		updated |= ImGui::InputFloat("Field of View", &m_FOV, flags);

		if(ImGui::Checkbox("Is Main Camera", &m_isMainCamera))
		{
			if(m_isMainCamera)
				SYSTEMS::GetObject().RENDERER.UpdateProjectionMatrix(m_projectionMatrix);
		}


		if(updated)
			OnIsDirty();


		if (ImGui::TreeNode("Preview current projection matrix"))
		{
			const float* data = glm::value_ptr(m_projectionMatrix);


			ImGui::Text("%.3f    %.3f    %.3f    %.3f"		, data[0]
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