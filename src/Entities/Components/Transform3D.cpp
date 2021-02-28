#include "Transform3D.hpp"
#include "glm.hpp"
#include "ext.hpp"
#include "GameObject.hpp"
#include "imgui.h"
#define M_PIf           3.14159265358979323846f  /* pi */

namespace asapi
{
	Transform3D::Transform3D(bfu::MemBlockBase* mBlock)
		:ComponentBase<Transform3D>(mBlock)
		,m_position("m_position", this, 3, mBlock)
		,m_rotation("m_rotation", this, 3, mBlock)
		,m_scale("m_scale", this, 3, mBlock)
	{
		m_scale[0] = 1.0f;
		m_scale[1] = 1.0f;
		m_scale[2] = 1.0f;
	};

	void Transform3D::OnAttach()
	{
		OnIsDirty();
	}

	void Transform3D::OnIsDirty()
	{
		//log::debug << "UpdateModelMatrix" << std::endl;1
		GameObject* parent = m_owner->GetParent();

		m_modelMatix = parent!=0 ? parent->GetTransform3D()->m_modelMatix : glm::mat4(1.0f);

		m_modelMatix = glm::translate(m_modelMatix, glm::vec3( m_position[0], m_position[1], m_position[2] ) );
		//m_modelMatix = glm::translate(m_modelMatix, localPivot * glm::vec3(1.0f, -1.0f, 1.0f));
		m_modelMatix = glm::rotate(m_modelMatix, m_rotation[1]*M_PIf/180.0f, glm::vec3(0.0, 1.0, 0.0));
		m_modelMatix = glm::rotate(m_modelMatix, m_rotation[0]*M_PIf/180.0f, glm::vec3(1.0, 0.0, 0.0));
		m_modelMatix = glm::rotate(m_modelMatix, m_rotation[2]*M_PIf/180.0f, glm::vec3(0.0, 0.0, 1.0));
		//m_modelMatix = glm::translate(m_modelMatix, localPivot * glm::vec3(-1.0f, 1.0f, -1.0f));

		// glm::mat4 View = glm::lookAt(
		//     glm::vec3(0.0f ,0.0f ,-5.0f ), // Camera is at (0,0,-5), in World Space
		//     glm::vec3(0.0f ,0.0f ,0.0f), // and looks at the origin
		//     glm::vec3(0.0f ,1.0f ,0.0f)  // Head is up (set to 0,-1,0 to look upside-down)
		//     );

		// glm::mat4 Projection = glm::perspective( 0.395f, 1.0f, 1.0f, 200.0f) * glm::ortho(0.0f, 1024.0f, 600.0f, 0.0f, 1.0f, -1.0f);	

	
		const int size = m_owner->GetChildCount();
		for(int i=0; i<size; ++i)
		{
			m_owner->GetChild(i)->GetTransform3D()->OnIsDirty(); // == OnIsDirty()
		}
	}

	void Transform3D::OnGUI()
	{
		bool updated = false;
		updated |= ImGui::InputFloat3("Position", &m_position[0]);
		updated |= ImGui::InputFloat3("Rotation", &m_rotation[0]);
		updated |= ImGui::InputFloat3("Scale", &m_scale[0]);


		if(updated)
			OnIsDirty(); // OnIsDirty()
	}
}