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
		char buff[255];
		strncpy(buff, m_meshName.c_str(), 255 );

		if( ImGui::InputText("MeshComponent##Mesh name",     buff, 255, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue) )
		{
			if( m_meshName.c_str() != buff )
			{
				m_meshName.clear();
				m_meshName.sprintf(buff);
			}

			log::debug << "updated mesh name " << buff << std::endl;

			OnIsDirty();			
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