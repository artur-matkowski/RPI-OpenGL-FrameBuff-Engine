#include "ComponentInterface.hpp"
#include "imgui.h"



namespace asapi
{
	TypeInfo 	a_typeInfo[TYPE_INFO_CAPACITY];
	int 		i_typeInfoCount = 0;




	void TypeInfo::RegisterType(InitFuncPtr fPtr, size_t id, size_t sizeOf, const char* name)
	{
		a_typeInfo[i_typeInfoCount].fPtr = fPtr;
		a_typeInfo[i_typeInfoCount].id = id;
		a_typeInfo[i_typeInfoCount].sizeOf = sizeOf;
		a_typeInfo[i_typeInfoCount].name = name;
		++i_typeInfoCount;
	}


	TypeInfo* TypeInfo::GetTypeInfo(size_t in)
	{
		for(int i = 0; i<i_typeInfoCount; ++i)
		{
			if(a_typeInfo[i].id == in)
			{
				return &a_typeInfo[i];
			}
		}
		return nullptr;
	}

	TypeInfo* TypeInfo::GetTypeInfo(const char* in)
	{
		for(int i = 0; i<i_typeInfoCount; ++i)
		{
			if( strcmp(a_typeInfo[i].name, in) == 0 )
			{
				return &a_typeInfo[i];
			}
		}
		return nullptr;
	}

	TypeInfo* TypeInfo::GetTypeInfo()
	{
		return a_typeInfo;
	}
	int TypeInfo::GetTypeInfoSize()
	{
		return i_typeInfoCount;
	}





	// void ComponentInterface::OnGUInamed()
	// {
	// 	//TODO render serializable fields in imgui
		
	// 	ImGui::Text("Debug Component renderer for component %s", ComponentName); 
	// }

	void ComponentInterface::Attached(GameObject* owner)
	{
		m_owner = owner;
		this->OnAttach();	
	}
	void ComponentInterface::Detached()
	{
		this->OnDetach();	
	}

	#ifdef IS_EDITOR

	void ComponentInterface::OnGUI()
	{
		ImGui::LabelText( "Component", this->TypeName() ); 

		for(int i=0; i<v_SerializableRenderers.size(); ++i)
			v_SerializableRenderers[i]->OnGUI();
	}

	void ComponentInterface::PushSerializableRenderer(SerializableRendererBase* in)
	{
		v_SerializableRenderers.push_back(in);
	}
	#endif

}