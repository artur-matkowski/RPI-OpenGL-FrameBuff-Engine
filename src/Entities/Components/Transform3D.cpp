#include "Transform3D.hpp"
#include "glm.hpp"
#include "ext.hpp"
#include "GameObject.hpp"
#include "imgui.h"

namespace asapi
{
	Transform3D::Transform3D(bfu::MemBlockBase* mBlock)
		:ComponentBase<Transform3D>(mBlock)
		,m_position("m_position", this, 3, mBlock)
		,m_rotation("m_rotation", this, 3, mBlock)
		,m_scale("m_scale", this, 3, mBlock)
	{
		m_modelMatix = glm::mat4(1.0f);
		m_modelMatix = glm::translate(m_modelMatix, glm::vec3( m_position[0], m_position[1], m_position[2] ) );
		m_modelMatix = glm::rotate(m_modelMatix, m_rotation[1], glm::vec3(0.0, 1.0, 0.0));
		m_modelMatix = glm::rotate(m_modelMatix, m_rotation[0], glm::vec3(1.0, 0.0, 0.0));
		m_modelMatix = glm::rotate(m_modelMatix, m_rotation[2], glm::vec3(0.0, 0.0, 1.0));
	};

	void Transform3D::UpdateModelMatrix()
	{
		//log::debug << "UpdateModelMatrix" << std::endl;
		GameObject* parent = m_owner->GetParent();

		m_modelMatix = parent!=0 ? parent->GetTransform3D()->m_modelMatix : glm::mat4(1.0f);

		m_modelMatix = glm::translate(m_modelMatix, glm::vec3( m_position[0], m_position[1], m_position[2] ) );
		//m_modelMatix = glm::translate(m_modelMatix, localPivot * glm::vec3(1.0f, -1.0f, 1.0f));
		m_modelMatix = glm::rotate(m_modelMatix, m_rotation[1], glm::vec3(0.0, 1.0, 0.0));
		m_modelMatix = glm::rotate(m_modelMatix, m_rotation[0], glm::vec3(1.0, 0.0, 0.0));
		m_modelMatix = glm::rotate(m_modelMatix, m_rotation[2], glm::vec3(0.0, 0.0, 1.0));
		//m_modelMatix = glm::translate(m_modelMatix, localPivot * glm::vec3(-1.0f, 1.0f, -1.0f));


		const int size = m_owner->GetChildCount();
		for(int i=0; i<size; ++i)
		{
			m_owner->GetChild(i)->GetTransform3D()->UpdateModelMatrix();
		}
	}


	void Transform3D::OnAttach()
	{
		UpdateModelMatrix();
	}
	void Transform3D::OnGUI()
	{
		bool updated = false;
		updated |= ImGui::InputFloat3("Position", &m_position[0]);
		updated |= ImGui::InputFloat3("Rotation", &m_rotation[0]);
		updated |= ImGui::InputFloat3("Scale", &m_scale[0]);

		if(updated)
			UpdateModelMatrix();
	}
}