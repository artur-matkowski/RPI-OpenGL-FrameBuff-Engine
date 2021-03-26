#include "GameObject.hpp"
#include "Systems.hpp"
#include "imgui.h"
#include "PrefabLoaderComponent.hpp"

namespace asapi
{
	bool GameObject::RegisterChild(GameObject* newChild)
	{
		v_children.push_back( newChild );
		return true;
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
		//,m_myName("m_myName", this, mBlock)
		//,v_componentsInfo("v_componentsInfo", this, SYSTEMS::JSON_ALLOCATOR ) //it is only usefull when de/serializing JSON
		,v_components(mBlock)
	{
		// m_myName.resize(GAMEOBJECT_MAX_NAME_LENGTH, '\0');
		m_myName = "GameObject";

		// v_children = (bfu::SerializableVarVector<GameObject*>*)mBlock->allocate( 1
		// 								, sizeof(bfu::SerializableVarVector<GameObject*>)
		// 				 				, alignof(bfu::SerializableVarVector<GameObject*>) );
		// new (v_children) bfu::SerializableVarVector<GameObject*>("v_children", this, mBlock);

		// AddComponent( TypeInfo::GetTypeInfo("asapi::Transform3D")->id );
	}

	GameObject::~GameObject()
	{
		ClearComponents();
		ClearChildren();
	}
	void GameObject::ClearChildren()
	{
		// if(v_children==nullptr)
		// 	return;

		// for(auto it = v_children->begin(); 
		// 	it != v_children->end();
		// 	++it)
		// {
		// 	(*it)->DispouseAndDeallocate();
		// }
		// v_children->clear();
	}
	void GameObject::ClearComponents()
	{
		while( v_components.size() != 0 )
		{
			ComponentInterface* ptr = *v_components.begin();
			v_components.erase( v_components.begin() );
			ptr->Detached();
			m_mBlock->deallocate(ptr, TypeInfo::GetTypeInfo( ptr->TypeHash() )->sizeOf ); // TODO wrong sizeof
		}
	}
	void GameObject::AddChild()
	{
		PrefabLoaderComponent* plc = GetRegionalPrefabLoader();
		PrefabMemBlock* allocator = plc->RequestPrefabMemBlock();

		GameObject *newChild = (GameObject*)allocator->allocate(1, sizeof(GameObject), alignof(GameObject));
		newChild->Init( this->GetMemBlock() );
		newChild->OnAttach(this);
	}

	//TODO copy all children, and all components
	// GameObject& GameObject::operator=(const GameObject& cp)
	// {
	// 	Dispouse();
	// 	Init(cp.m_mBlock);
	// 	m_myName = "Copy of " + cp.m_myName;
	// 	OnAttach(cp.p_parent);

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


	void GameObject::PreDeserializationCallback()
	{

	}
	void GameObject::PostDeserializationCallback()
	{

	}
	void GameObject::PreSerializationCallback()
	{

	}
	void GameObject::PostSerializationCallback()
	{

	}

	void GameObject::PopulateComponentInfo()
	{
		ClearComponentInfo();

		// for(int i=0; i<v_components.size(); ++i)
		// {
		// 	v_componentsInfo.emplace_back( SYSTEMS::STD_ALLOCATOR );

		// 	v_componentsInfo.back().m_componentTypeName.GetRef().sprintf( v_components[i]->TypeName() );
		// 	//v_componentsInfo.back().m_recreationString << *v_components[i]; //Can't really do that is we have a ptr not a full reference
		// 	v_components[i]->Serialize( v_componentsInfo.back().m_recreationString.GetRef() );
		// }
	}
	void GameObject::ClearComponentInfo()
	{
		//v_componentsInfo.clear();
	}
	void GameObject::ReconstructComponentsFromComponentInfo()
	{
		ClearComponents();

		// for(int i=0; i<v_componentsInfo.size(); ++i)
		// {
		// 	this->AddComponent( v_componentsInfo[i].m_componentTypeName.GetRef().c_str() );			

		// 	auto &recreationString = v_componentsInfo[i].m_recreationString.GetRef();
		// 	if(recreationString.size() > 0)
		// 	{
		// 		v_components.back()->Deserialize( v_componentsInfo[i].m_recreationString.GetRef() );
		// 		v_components.back()->OnIsDirty();
		// 	}
		// }
		ClearComponentInfo();
	}



/*
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
    	char buff[1024] = {'0'};
    	bfu::stream token(buff, 1024, m_mBlock);
    	
		//this->EntityBase::Deserialize(stream);	
		{
			stream.skipTo('{');
			stream.skip( 1 );

			if(stream.peak() == '\n')
				stream.skip( 1 );

			while( stream.peak() != '}' )
			{
				token.clear();

				stream.Deserialize( token );

				auto it = m_membersMap.find( token );
				if( it == m_membersMap.end() )
				{
					log::error << "co do kurwy " << token.c_str() << std::endl;
				}

				it->second->Deserialize( stream );

				stream.skipToOneOf("\"}");

			}
			stream.skip(1);
		}

		ReconstructComponentsFromComponentInfo();

		if(v_children!=nullptr)
		for(int i=0; i<v_children->size(); ++i)
		{
			//OnAttach
			(*v_children)[i]->p_parent = this;
		}
	}
*/
	void GameObject::SerializeChildren(bfu::JSONStream& stream)
	{
		//stream << *v_children;
	}
	void GameObject::DeserializeChildren(bfu::JSONStream& stream)
	{
		// stream >> *v_children;

		// if(v_children!=nullptr)
		// for(int i=0; i<v_children->size(); ++i)
		// {
		// 	//OnAttach
		// 	(*v_children)[i]->p_parent = this;
		// }
	}


	void GameObject::OnAttach(GameObject* newParent)
	{
		p_parent = newParent;
		newParent->RegisterChild( this );
	}
	void GameObject::OnDetach()
	{
		p_parent->UnRegisterChild( this );
		p_parent = 0;
	}
	void GameObject::ReAttach(GameObject* newParent)
	{
		p_parent->UnRegisterChild( this );
		p_parent = newParent;
		newParent->RegisterChild( this );
	}
	void GameObject::OverrideChildVector(bfu::SerializableVarVector<GameObject*>* newChildrenVector, bfu::MemBlockBase* prefabMemBlock )
	{
		// if(v_children!=nullptr)
		// {
		// 	v_children->~SerializableVarVector<GameObject*>();
		// 	m_mBlock->deallocate( v_children, 1*sizeof(bfu::SerializableVarVector<GameObject*>) );
		// }

		// v_children = newChildrenVector;
	}

	ComponentInterface* GameObject::AddComponent(const char* componentName)
	{
		return AddComponent( TypeInfo::GetTypeInfo( componentName )->id );
	}

	ComponentInterface* GameObject::AddComponent(size_t typeHash)
	{
		if( GetComponentOfTypeHash(typeHash) != nullptr)
			return nullptr;

		ComponentInterface* newComp = ComponentInterface::AllocateAndInitObjectFromTypeHash(typeHash, m_mBlock);


		if( newComp->TypeHash()			== typeid(Transform3D).hash_code() )
		{
			p_myTransform = (Transform3D*)newComp;
		}
		else if( newComp->TypeHash()	== typeid(RendererComponent).hash_code() )
		{
			p_myRenderer = (RendererComponent*)newComp;
		}

		v_components.push_back(newComp);

		newComp->Attached(this);

		return newComp;
	}
	void GameObject::RemoveComponent(ComponentInterface* ptr)
	{
		if( ptr->TypeHash() == typeid(Transform3D).hash_code() )
		{
			log::warning << "Can not remove Transform3D. One has to be present on GameObject node" << std::endl;
			return;
		}

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
	PrefabLoaderComponent* GameObject::GetRegionalPrefabLoader()
	{
		for(GameObject* ptr = this; ptr!=nullptr; ptr = ptr->GetParent())
		{
			PrefabLoaderComponent* ret = (PrefabLoaderComponent*)ptr->GET_COMPONENT(PrefabLoaderComponent);
			if( ret!= nullptr )
			{
				return ret;
			}
		}
		ASSERT(true, "Could not find PrefabLoaderComponent in region");
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