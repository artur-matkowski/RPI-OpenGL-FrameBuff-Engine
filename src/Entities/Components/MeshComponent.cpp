#include "MeshComponent.hpp"
#ifdef IS_EDITOR
#include "imgui.h"
#endif
#include "Systems.hpp"

namespace asapi
{
	MeshComponent::MeshComponent(bfu::MemBlockBase* mBlock)
		:ComponentBase<MeshComponent>(mBlock)
		,m_meshName(buffMesh, 255, mBlock)
		,m_mesh()
	{
	};

	void MeshComponent::OnIsDirty()
	{
		SYSTEMS& systems = SYSTEMS::GetObject();
		systems.ASSETS.requestResource( &m_mesh, m_meshName.c_str() );
	}

	#ifdef IS_EDITOR
	void MeshComponent::OnGUI()
	{
		std::vector<std::string>* items = SYSTEMS::GetObject().ASSETS.GetMeshesPaths();

		if (ImGui::BeginCombo("Mesh_old resource", m_meshName.c_str()))
        {
            for (int n = 0; n < items->size(); n++)
            {
            	const char* displayName = strstr( (*items)[n].c_str(), "/meshes/") + strlen("/meshes/");
                const bool is_selected = strcmp( m_meshName.c_str(), (*items)[n].c_str() ) == 0;
                if (ImGui::Selectable(displayName, is_selected))
                {
                	if( m_meshName.c_str() != displayName )
					{
						m_meshName.clear();
						m_meshName.sprintf(displayName);
					}

					log::debug << "updated mesh name " << displayName << std::endl;

					OnIsDirty();	
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

		Mesh_old* ptr = (Mesh_old*)m_mesh.GetRawPtr();
		if( ptr!=nullptr && ptr->GetRawHandle() != nullptr )
		{
			ImGui::Text("Mesh_old is properly loaded to GPU");
		}
		else
		{
			ImGui::Text("Mesh_old is invalid");
		}
		
		ComponentBase<MeshComponent>::OnGUI();
	}
	#endif
}