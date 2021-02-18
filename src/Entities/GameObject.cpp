#include "GameObject.hpp"
#include "Systems.hpp"
#include "imgui.h"

namespace asapi
{
	void GameObject::RegisterChild(GameObject* newChild)
	{
		v_children.push_back( newChild );
	}

	void GameObject::UnRegisterChild(GameObject* deleteChild)
	{
		for(auto it = v_children.begin(); 
			it != v_children.end();
			++it)
		{
			if(*it==deleteChild)
			{
				v_children.erase(it);
				break;
			}
		}
	}

	GameObject::GameObject( bfu::MemBlockBase* mBlock )
		:EntityBase(mBlock)
		,b_isGameObjectLoader(false)
		,m_myName("m_myName", this, mBlock)
		,v_children("v_children", this, this, mBlock)
		,v_componentsInfo("v_componentsInfo", this, mBlock)
		,v_components(mBlock)
	{
		m_myName.resize(GAMEOBJECT_MAX_NAME_LENGTH, '\0');
		m_myName = "GameObject";
	}

	GameObject::GameObject( const GameObject& cp )
		:EntityBase(cp)
		,b_isGameObjectLoader(false)
		,p_parrent(cp.p_parrent)
		,m_myName("m_myName", this, cp.m_mBlock)
		,v_children("v_children", this, this, cp.m_mBlock)
		,v_componentsInfo("v_componentsInfo", this, cp.m_mBlock)
		,v_components(cp.m_mBlock)
	{
		m_myName.resize(GAMEOBJECT_MAX_NAME_LENGTH, '\0');
		m_myName = cp.m_myName.c_str();
	}

	GameObject::~GameObject()
	{
		for(auto it = v_children.begin(); 
			it != v_children.end();
			++it)
		{
			(*it)->DispouseAndDeallocate();
		}
	}

	GameObject& GameObject::operator=(const GameObject& cp)
	{
		this->~GameObject();
		new (this) GameObject(cp);
		return *this;
	}

	void GameObject::Init( bfu::MemBlockBase* mBlock )
	{
		new (this) GameObject(mBlock);
	}
	void GameObject::Dispouse()
	{
		this->~GameObject();
	}
	void GameObject::DispouseAndDeallocate()
	{
		this->~GameObject();
		m_mBlock->deallocate(this, sizeof(GameObject));
	}

	void GameObject::OnLoad()
	{

	}
	void GameObject::OnUnLoad()
	{

	}

	void GameObject::Serialize()
	{
		//we will pe using GameObject name as prefab file name
		char filePath[GAMEOBJECT_MAX_NAME_LENGTH+sizeof(SERIALIZATION_FILE_EXT)+sizeof(SERIALIZATION_FOLDER)]; 
		bfu::JSONStream& jsonStream = SYSTEMS::GetObject().SCENE.GetJSONStreamWorkBuffer();

		//building a file name;
		strcpy(filePath, SERIALIZATION_FOLDER );
		strncpy(filePath+sizeof(SERIALIZATION_FOLDER)-1, m_myName.c_str(), GAMEOBJECT_MAX_NAME_LENGTH );
		strncpy(filePath+sizeof(SERIALIZATION_FOLDER)-1+m_myName.size(), SERIALIZATION_FILE_EXT, sizeof(SERIALIZATION_FILE_EXT));

		FILE * pFile = fopen (filePath,"wb");

		if( pFile==NULL )
		{
			log::error << "Could not open file: " << filePath << std::endl;
			return;
 		}

 		jsonStream << v_children;

		fwrite(jsonStream.c_str(), 1, jsonStream.size(), pFile);

		fclose (pFile);

		jsonStream.clear();
	}
	void GameObject::Deserialize()
	{
		//we will pe using GameObject name as prefab file name
		char filePath[GAMEOBJECT_MAX_NAME_LENGTH+sizeof(SERIALIZATION_FILE_EXT)+sizeof(SERIALIZATION_FOLDER)]; 
		bfu::JSONStream& jsonStream = SYSTEMS::GetObject().SCENE.GetJSONStreamWorkBuffer();

		//building a file name;
		strcpy(filePath, SERIALIZATION_FOLDER );
		strncpy(filePath+sizeof(SERIALIZATION_FOLDER)-1, m_myName.c_str(), GAMEOBJECT_MAX_NAME_LENGTH );
		strncpy(filePath+sizeof(SERIALIZATION_FOLDER)-1+m_myName.size(), SERIALIZATION_FILE_EXT, sizeof(SERIALIZATION_FILE_EXT));

		FILE * pFile = fopen (filePath,"rb");

		if( pFile==NULL )
		{
			log::error << "Could not open file: " << filePath << std::endl;
			return;
 		}
		
		fseek(pFile, 0L, SEEK_END); 
		auto fileSize = ftell(pFile); 
		fseek(pFile, 0L, SEEK_SET); 

		jsonStream.resize(fileSize);
		fread(jsonStream.c_str(), 1, fileSize, pFile);
		jsonStream.OverrideWriteCursorPos(fileSize);

 		jsonStream >> v_children;

		fclose (pFile);

		jsonStream.clear();
	}


	void GameObject::OnAttach(GameObject* newParrent)
	{
		p_parrent = newParrent;
		newParrent->RegisterChild( this );
	}
	void GameObject::OnDetach()
	{
		p_parrent->UnRegisterChild( this );
		p_parrent = 0;
	}
	void GameObject::ReAttach(GameObject* newParrent)
	{
		p_parrent->UnRegisterChild( this );
		p_parrent = newParrent;
		newParrent->RegisterChild( this );
	}




	void GameObject::SetName(const char* name)
	{
		m_myName = name;
	}


	void GameObject::OnGUI()
	{
		ImGui::Spacing();
		ImGui::Text( GetName() );

		for(int i=0; i<v_components.size(); ++i)
		{
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			v_components[i]->OnGUI();
		}

		
	}
}