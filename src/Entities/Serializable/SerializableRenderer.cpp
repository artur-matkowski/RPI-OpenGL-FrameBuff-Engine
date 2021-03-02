#include "SerializableRenderer.hpp"
#ifdef IS_EDITOR
#include "Serializable.hpp"
#include "imgui.h"

namespace asapi
{
	SerializableRendererBase::SerializableRendererBase(const char* name)
	{
		int nameI = 0;
		int posOfFirstUnderscore = 0;

		for(int i = 0; i<strlen(name); ++i)
		{
			if( name[i] == '_' )
			{
				posOfFirstUnderscore = i+1;
				break;
			}
		}

		m_name[nameI] = name[posOfFirstUnderscore];
		if( m_name[nameI]>= 'a' && m_name[nameI] <= 'z' )
			m_name[nameI] += 'A'-'a';
		++nameI;
		for(int i = posOfFirstUnderscore+1; i<strlen(name); ++i)
		{
			if( name[i] >= 'A' && name[i] <= 'Z' && m_name[nameI-1] != ' ')
			{
				m_name[nameI] = ' ';
				++nameI;
			}

			m_name[nameI] = name[i];
			++nameI;
			
		}
	}

	void SerializableRendererBase::OnGUI()
	{
		ImGui::Text("Serializable Renderer not implemented for type of %s", m_name); 
	}


	template<>
	void SerializableRenderer< Serializable<bfu::string> >::OnGUI()
	{
		char buff[1024];
		strncpy(buff, m_obj.GetRef().c_str(), 1024 );

		ImGui::InputText(m_name,     buff, 1024);

		m_obj.GetRef() = buff;
	}

	template<>
	void SerializableRenderer< Serializable<bfu::stream> >::OnGUI()
	{
		char buff[1024];
		strncpy(buff, m_obj.GetRef().c_str(), 1024 );

		ImGui::InputText(m_name,     buff, 1024);

		m_obj.GetRef().clear();
		m_obj.GetRef().sprintf( buff );
	}

	template<>
	void SerializableRenderer< Serializable<bool> >::OnGUI()
	{
		ImGui::Checkbox(m_name, &m_obj.GetRef() );
	}

	template<>
	void SerializableRenderer< Serializable<float> >::OnGUI()
	{
		ImGui::InputFloat(m_name, &m_obj.GetRef() );
	}

	template<>
	void SerializableRenderer< Serializable<int> >::OnGUI()
	{
		ImGui::InputInt(m_name, &m_obj.GetRef() );
	}

	template<>
	void SerializableRenderer< SerializableArray<float> >::OnGUI()
	{
		const int size = m_obj.size();

		switch(size)
		{
			case 1:
				ImGui::InputFloat(m_name, &m_obj[0]);
				break;
			case 2:
				ImGui::InputFloat2(m_name, &m_obj[0]);
				break;
			case 3:
				ImGui::InputFloat3(m_name, &m_obj[0]);
				break;
			case 4:
				ImGui::InputFloat4(m_name, &m_obj[0]);
				break;
			default:
				ImGui::Text("Serializable Renderer not implemented for type of %s and size of %d", m_name, size); 
		}
	}

	template<>
	void SerializableRenderer< SerializableArray<int> >::OnGUI()
	{
		const int size = m_obj.size();

		switch(size)
		{
			case 1:
				ImGui::InputInt(m_name, &m_obj[0]);
				break;
			case 2:
				ImGui::InputInt2(m_name, &m_obj[0]);
				break;
			case 3:
				ImGui::InputInt3(m_name, &m_obj[0]);
				break;
			case 4:
				ImGui::InputInt4(m_name, &m_obj[0]);
				break;
			default:
				ImGui::Text("Serializable Renderer not implemented for type of %s and size of %d", m_name, size); 
		}
	}


}

#endif