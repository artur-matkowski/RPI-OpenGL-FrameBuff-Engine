#include "GameObject.hpp"
#include "Systems.hpp"
#include "imgui.h"
#include "PrefabLoaderComponent.hpp"

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

	GameObject::GameObject(  )
		:EntityBase(&SYSTEMS::GetObject().MEMORY.GetStdAllocator())
		,b_isGameObjectLoader(false)
		,m_myName("m_myName", this, &SYSTEMS::GetObject().MEMORY.GetStdAllocator())
		,v_children("v_children", this, &SYSTEMS::GetObject().MEMORY.GetStdAllocator())
		,v_componentsInfo("v_componentsInfo", this, &SYSTEMS::GetObject().MEMORY.GetStdAllocator() ) //it is only usefull when de/serializing JSON
		,v_components(&SYSTEMS::GetObject().MEMORY.GetStdAllocator())
	{
		m_myName.resize(GAMEOBJECT_MAX_NAME_LENGTH, '\0');
		m_myName = "GameObject";

		AddComponent( TypeInfo::GetTypeInfo("asapi::Transform3D")->id );
	}

	GameObject::GameObject( bfu::MemBlockBase* mBlock )
		:EntityBase(mBlock)
		,b_isGameObjectLoader(false)
		,m_myName("m_myName", this, mBlock)
		,v_children("v_children", this, mBlock)
		,v_componentsInfo("v_componentsInfo", this, &SYSTEMS::GetObject().MEMORY.GetStdAllocator() ) //it is only usefull when de/serializing JSON
		,v_components(mBlock)
	{
		m_myName.resize(GAMEOBJECT_MAX_NAME_LENGTH, '\0');
		m_myName = "GameObject";

		AddComponent( TypeInfo::GetTypeInfo("asapi::Transform3D")->id );
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

	//TODO copy all children, and all components
	// GameObject& GameObject::operator=(const GameObject& cp)
	// {
	// 	Dispouse();
	// 	Init(cp.m_mBlock);
	// 	m_myName = "Copy of " + cp.m_myName;
	// 	OnAttach(cp.p_parrent);

	// 	return *this;
	// }

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

	void GameObject::PopulateComponentInfo()
	{
		ClearComponentInfo();

		for(int i=0; i<v_components.size(); ++i)
		{
			v_componentsInfo.emplace_back( &SYSTEMS::GetObject().MEMORY.GetStdAllocator() );

			v_componentsInfo.back().m_typeId = v_components[i]->TypeHash();
			//v_componentsInfo.back().m_recreationString << *v_components[i]; //Can't really do that is we have a ptr not a full reference
			v_components[i]->Serialize( v_componentsInfo.back().m_recreationString.GetRef() );
		}
	}
	void GameObject::ClearComponentInfo()
	{
		v_componentsInfo.clear();
	}
	void GameObject::ReconstructComponentsFromComponentInfo()
	{
		v_components.clear();

		for(int i=0; i<v_componentsInfo.size(); ++i)
		{
			this->AddComponent( v_componentsInfo[i].m_typeId );
			auto &recreationString = v_componentsInfo[i].m_recreationString.GetRef();
			if(recreationString.size() > 0)
				v_components.back()->Deserialize( v_componentsInfo[i].m_recreationString.GetRef() );
		}
		ClearComponentInfo();
	}




	void GameObject::Serialize(bfu::JSONStream& stream)
	{
		PopulateComponentInfo();

		ComponentInterface* loader = this->GET_COMPONENT(PrefabLoaderComponent);

		//this->EntityBase::Serialize(stream);	
		{
			stream.sprintf("{");

			auto last = m_membersMap.end();

			for(auto it = m_membersMap.begin(); it != last; )
			{
				if( loader!=0 && strcmp(it->first.c_str(), "v_children")==0 ){
					++it;
					continue;
				}

				stream.sprintf("\n\"%s\": ", it->first.c_str() );

				it->second->Serialize( stream );

				++it;

				if( it != last )
				{
					stream.sprintf(", ");
				}
			}

			stream.sprintf("\n}");
		}
	}
	void GameObject::Deserialize(bfu::JSONStream& stream)
	{
		//this->EntityBase::Deserialize(stream);	
		{
			stream.skipTo('{');
			stream.skip( 1 );

			if(stream.peak() == '\n')
				stream.skip( 1 );

			while( stream.peak() != '}' )
			{
				m_token.clear();

				stream.Deserialize( m_token );

				auto &tmp = m_membersMap[ m_token.str() ];

				tmp->Deserialize( stream );

				stream.skipToOneOf("\"}");

			}
			stream.skip(1);
		}

		ReconstructComponentsFromComponentInfo();
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


	ComponentInterface* GameObject::AddComponent(size_t typeHash)
	{
		if( GetComponentOfTypeHash(typeHash) != nullptr)
			return nullptr;

		ComponentInterface* newComp = ComponentInterface::AllocateAndInitObjectFromTypeHash(typeHash, m_mBlock);

		v_components.push_back(newComp);

		newComp->Attached(this);

		return newComp;
	}
	void GameObject::RemoveComponent(ComponentInterface* ptr)
	{
		for(auto it = v_components.begin(); it!=v_components.end(); ++it)
		{
			if(*it==ptr)
			{
				v_components.erase(it);
				ptr->Detached();
				m_mBlock->deallocate(ptr, TypeInfo::GetTypeInfo( ptr->TypeHash() )->sizeOf ); // TODO wrong sizeof
				break;
			}
		}
	}
	ComponentInterface* GameObject::GetComponentOfTypeHash(size_t typeHash)
	{
		for(auto it = v_components.begin(); it!=v_components.end(); ++it)
		{
			if( (*it)->TypeHash() == typeHash)
			{
				return *it;
			}
		}

		return nullptr;
	}


	void GameObject::SetName(const char* name)
	{
		m_myName = name;
	}


	#ifdef IS_EDITOR
	void GameObject::OnGUI()
	{
		ImGui::Spacing();
		ImGui::Text( GetName() );

		for(int i=0; i<v_components.size(); ++i)
		{
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			v_components[i]->OnGUI_NameAndVirtual();
		}
	}
	#endif
}