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
	{
	};

	void MeshComponent::OnIsDirty()
	{
		SYSTEMS& systems = SYSTEMS::GetObject();
		systems.RESOURCES.requestResource( &m_mesh, m_meshName.c_str() );
	}

	#ifdef IS_EDITOR
	void MeshComponent::OnGUI()
	{
		std::vector<std::string>* items = SYSTEMS::GetObject().RESOURCES.GetMeshesPaths();

		if (ImGui::BeginCombo("Mesh resource", m_meshName.c_str()))
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

		Mesh* ptr = (Mesh*)m_mesh.GetRawPtr();
		if( ptr!=nullptr && ptr->GetRawHandle() != nullptr )
		{
			ImGui::Text("Mesh is properly loaded to GPU");
		}
		else
		{
			ImGui::Text("Mesh is invalid");
		}
	}
	#endif
}