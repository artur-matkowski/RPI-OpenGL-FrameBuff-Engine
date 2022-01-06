#include "Systems.hpp"
#include "RendererComponent.hpp"
#include "GameObject.hpp"
#include "ext.hpp"
#ifdef IS_EDITOR
#include "imgui.h"
#endif
#include <GLES2/gl2.h>

namespace asapi
{
	RendererComponent::RendererComponent(bfu::MemBlockBase* mBlock)
		:ComponentBase<RendererComponent>(mBlock)
		,m_MaterialName(buffMat, 255, mBlock)
	{
	};

	void RendererComponent::OnAttach()
	{
		p_modelViewMat = ((Transform3D*)m_owner->GET_COMPONENT(Transform3D))->GetModelMatrix();
		#ifdef IS_EDITOR
		SYSTEMS::GetObject().ASSETS.RegisterRendererComponent( this );
		#endif

	}
	void RendererComponent::OnDetach()
	{
		SYSTEMS::GetObject().RENDERER.UnRegisterRenderer( this );
		#ifdef IS_EDITOR
		SYSTEMS::GetObject().ASSETS.UnRegisterRendererComponent( this );
		#endif
	}
	void RendererComponent::OnIsDirty()
	{
		SYSTEMS& systems = SYSTEMS::GetObject();
		
		if( m_material.IsValid() )
		{
			/* TODO
			if( strcmp( m_material->GetMaterialName(), m_MaterialName.c_str() )!=0 )
			{
				systems.ASSETS.requestResource( &m_material, m_MaterialName.c_str() );
			}*/
		}
		else
		{
			systems.ASSETS.requestResource( &m_material, m_MaterialName.c_str() );			
		}

		p_meshComponent = (MeshComponent*)m_owner->GET_COMPONENT(MeshComponent);

		if( m_material.GetRawPtr() != nullptr 
			&& m_material->IsValid() 
			&& p_meshComponent != nullptr
			&& p_meshComponent->GetMeshHandle() != nullptr )
		{
			//if you would ever try to update renderer component on the fly, you need to first unregister it from 
			//renderer system, to unbound mesh from material render queue.
			SYSTEMS::GetObject().RENDERER.RegisterRenderer( this );
		}
	}

	void RendererComponent::Render(glm::mat4* projectionMatrix, glm::mat4* viewMatrix)
	{
		m_material->GetModelViewMatrix()->SetUniform( *projectionMatrix * *viewMatrix * *p_modelViewMat );
		m_material->BindMaterial();

		uint32_t* config = (uint32_t*)p_meshComponent->GetMeshHandle();

		// //Render
		glBindBuffer(GL_ARRAY_BUFFER, config[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, config[1]);

		const uint32_t attributes = config[2];
		for(uint32_t i = 0; i<attributes; ++i)
		{
			const uint32_t index = i*5+3;
			glEnableVertexAttribArray(config[index+0]);
			glVertexAttribPointer(config[index+1]
								, config[index+2]
								, GL_FLOAT
								, GL_FALSE
								, sizeof(GL_FLOAT)*(config[index+3])
								, (void*) (sizeof(GL_FLOAT)*(config[index+4])) );
		}


		const uint32_t size = config[3+attributes*5];
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, NULL);

		// glDisableVertexAttribArray(0);
		// glDisableVertexAttribArray(2);
	}
	
	void RendererComponent::PreSerializationCallback()
	{
		m_MaterialName.clear();
		#ifdef IS_EDITOR
		m_MaterialName.sprintf(m_material->GetMaterialName());
		#endif
	}

	#ifdef IS_EDITOR
	void RendererComponent::OnGUI()
	{
		std::vector<std::string>* items = SYSTEMS::GetObject().ASSETS.GetMaterialsPaths();
        bool isAltered = false;


        if( strcmp(m_MaterialName.c_str(), m_material->GetMaterialName())!=0 )
        {
			m_MaterialName.clear();
			m_MaterialName.sprintf(m_material->GetMaterialName());
        }

		if (ImGui::BeginCombo("Material resource", m_MaterialName.c_str() ))
        {
            for (int n = 0; n < items->size(); n++)
            {
            	const char* displayName = strstr( (*items)[n].c_str(), "/materials/") + strlen("/materials/");
                const bool is_selected = strcmp( m_MaterialName.c_str(), (*items)[n].c_str() ) == 0;
                if (ImGui::Selectable(displayName, is_selected))
                {
                	if( strcmp(m_MaterialName.c_str(), displayName)!=0 )
                	{
						log::debug << "updating material name from: " << m_MaterialName.c_str() << " to: " << displayName << std::endl;

						m_MaterialName.clear();
						m_MaterialName.sprintf(displayName);


						OnIsDirty();

						isAltered = true;
                	}
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }



		// {
		// 	char buff1[255];
		// 	strncpy(buff1, m_MaterialName.c_str(), 255 );

		// 	if( ImGui::InputText("Material name",     buff1, 255, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue) )
		// 	{
		// 		log::debug << "updated material name " << buff1 << std::endl;

		// 		m_MaterialName.clear();
		// 		m_MaterialName.sprintf(buff1);

		// 		OnIsDirty();
		// 	}

		// }
		if(m_material.GetRawPtr()!=nullptr)
			m_material->OnGUI();
	}
	#endif

}