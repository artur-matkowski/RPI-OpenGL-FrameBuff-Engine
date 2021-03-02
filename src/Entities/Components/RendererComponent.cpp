#include "Systems.hpp"
#include "RendererComponent.hpp"
#include "GameObject.hpp"
#include "glm.hpp"
#include "ext.hpp"
#ifdef IS_EDITOR
#include "imgui.h"
#endif

namespace asapi
{
	RendererComponent::RendererComponent(bfu::MemBlockBase* mBlock)
		:ComponentBase<RendererComponent>(mBlock)
		,m_MaterialName("m_MaterialName",this, buffMat, 255, mBlock)
		,m_meshName("m_meshName",this, buffMesh, 255, mBlock)
	{
	};

	void RendererComponent::OnAttach()
	{
		m_isAttached = true;

		if( m_material.GetRawPtr() != nullptr && m_mesh.GetRawPtr() != nullptr && m_isAttached)
		{
			SYSTEMS::GetObject().RENDERER.RegisterRenderer( this );
		}
	}
	void RendererComponent::OnDetach()
	{
		m_isAttached = false;
		SYSTEMS::GetObject().RENDERER.UnRegisterRenderer( this );
	}
	void RendererComponent::OnIsDirty()
	{
		SetMaterial_Blocking( m_MaterialName.GetRef().c_str() );
		SetMesh_Blocking( m_meshName.GetRef().c_str() );
	}

	void RendererComponent::SetMaterial_Blocking(const char* name)
	{
		SYSTEMS& systems = SYSTEMS::GetObject();
		systems.RESOURCES.requestResource( &m_material, name );
		p_modelViewMat = (Uniform<glm::mat4>*) m_material->GetUniformPtr("modelViewMat");

		if( m_material.GetRawPtr() != nullptr && m_mesh.GetRawPtr() != nullptr && m_isAttached)
		{
			SYSTEMS::GetObject().RENDERER.RegisterRenderer( this );
		}

		if( m_MaterialName.GetRef().c_str() != name )
		{
			m_MaterialName.GetRef().clear();
			m_MaterialName.GetRef().sprintf(name);
		}
	}
	void RendererComponent::SetMesh_Blocking(const char* name)
	{
		SYSTEMS& systems = SYSTEMS::GetObject();
		systems.RESOURCES.requestResource( &m_mesh, name );

		if( m_material.GetRawPtr() != nullptr && m_mesh.GetRawPtr() != nullptr && m_isAttached)
		{
			SYSTEMS::GetObject().RENDERER.RegisterRenderer( this );
		}

		if( m_meshName.GetRef().c_str() != name )
		{
			m_meshName.GetRef().clear();
			m_meshName.GetRef().sprintf(name);
		}
	}

	void RendererComponent::Render()
	{
		if(p_modelViewMat!=0)
			p_modelViewMat->SetUniform( m_owner->GetTransform3D()->GetModelMatrix() );
		else
			log::warning << "failing on matrix uniform update" << std::endl;


		m_material->BindMaterial();
		m_mesh->Render();
	}
	
	#ifdef IS_EDITOR
	void RendererComponent::OnGUI()
	{

		{
			char buff1[255];
			strncpy(buff1, m_MaterialName.GetRef().c_str(), 255 );

			if( ImGui::InputText("Material name",     buff1, 255, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue) )
			{
				log::debug << "updated material name " << buff1 << std::endl;

				SetMaterial_Blocking(buff1);
			}

		}
		if(m_material.GetRawPtr()!=nullptr)
			m_material->OnGUI();

		ImGui::Spacing();
		{
			char buff2[255];
			strncpy(buff2, m_meshName.GetRef().c_str(), 255 );

			if( ImGui::InputText("Mesh name",     buff2, 255, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue) )
			{
				log::debug << "updated mesh name " << buff2 << std::endl;

				SetMesh_Blocking(buff2);				
			}
		}

	}
	#endif

}