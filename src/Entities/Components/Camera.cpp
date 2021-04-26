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
		m_cameraMode = 0;
	};

	void Camera::OnIsDirty()
	{
		glm::ivec2 res;
		SYSTEMS::GetObject().RENDERER.GetResolution(res);
		if(m_cameraMode==0)
			m_projectionMatrix = glm::perspective( glm::radians(m_FOV), ((float)res.x)/res.y, m_nearPlane, m_farPlane );
		else if(m_cameraMode==1)
			m_projectionMatrix = glm::ortho( m_orthoLeft, m_orthoRight, m_orthoBottom, m_orthoTop, m_nearPlane, m_farPlane );

		if(m_isMainCamera)
			SYSTEMS::GetObject().RENDERER.UpdateProjectionMatrix(m_projectionMatrix);
	}

	#ifdef IS_EDITOR
	void Camera::OnGUI()
	{
		bool updated = false;
		const char* items[] = { "Perspective", "Ortho"};
        const char* combo_label = items[m_cameraMode];  // Label to preview before opening the combo (technically it could be anything)
        
		if (ImGui::BeginCombo("Camera mode", combo_label))
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                const bool is_selected = (m_cameraMode == n);
                if (ImGui::Selectable(items[n], is_selected))
                {
                    m_cameraMode = n;
                    updated |= true;
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }


		
		updated |= ImGui::InputFloat("Near Plane", &m_nearPlane);
		updated |= ImGui::InputFloat("Far Plane", &m_farPlane);

		if(m_cameraMode==0)
		{
			updated |= ImGui::InputFloat("Field of View", &m_FOV);
		}
		else if(m_cameraMode==1)
		{
			updated |= ImGui::InputFloat("Left", &m_orthoLeft);
			updated |= ImGui::InputFloat("Right", &m_orthoRight);
			updated |= ImGui::InputFloat("Bottom", &m_orthoBottom);
			updated |= ImGui::InputFloat("Top", &m_orthoTop);
		}

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