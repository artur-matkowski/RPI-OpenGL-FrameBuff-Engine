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
			ComponentInterface* ptr = v_components.begin()->p_ComponentInterface;
			v_components.erase( v_components.begin() );
			ptr->Detached();
			ptr->Dispouse();
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


	void GameObject::RegisterTransform3D(Transform3D* ptr)
	{
		p_myTransform = ptr;
	}
	void GameObject::RegisterRendererComponent(RendererComponent* ptr)
	{
		p_myRenderer = ptr;
	}
	void GameObject::RegisterPrefabLoaderComponent(PrefabLoaderComponent* ptr)
	{
		p_myPrefabLoader = ptr;
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
		v_componentsInfo.clear();
	}
	void GameObject::PostDeserializationCallback()
	{
		for(int i=0; i<v_componentsInfo.size(); ++i)
		{
			this->AddComponent( v_componentsInfo[i].m_componentTypeName.c_str() );			

			auto &recreationString = v_componentsInfo[i].m_recreationString;
			if(recreationString.size() > 0)
			{
				bfu2::JSONSerializer serializer( std::move( v_componentsInfo[i].m_recreationString ) );
				serializer.Deserialize( v_components[i].p_SerializableClassInterface );
				v_componentsInfo[i].m_recreationString = std::move( serializer );

				v_components.back().p_ComponentInterface->OnIsDirty();
			}
		}
		v_componentsInfo.clear();
	}
	void GameObject::PreSerializationCallback()
	{
		v_componentsInfo.clear();

		for(int i=0; i<v_components.size(); ++i)
		{
			ComponentInfo* obj = (ComponentInfo*) ComponentInfo::AllocateAndInit( SYSTEMS::JSON_ALLOCATOR );
			v_componentsInfo.push_back( obj );

			obj->m_componentTypeName.sprintf( v_components[i].p_ComponentInterface->TypeName() );

			bfu2::JSONSerializer serializer( std::move( obj->m_recreationString ) );
			serializer.Serialize( v_components[i].p_SerializableClassInterface );
			obj->m_recreationString = std::move( serializer );
		}
	}
	void GameObject::PostSerializationCallback()
	{
		v_componentsInfo.clear();
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

	void GameObject::SerializeChildren(bfu2::JSONSerializer& stream)
	{
		stream.Serialize( (bfu2::SerializableVector<SerializableClassInterface>*) &v_children );
	}
	void GameObject::DeserializeChildren(bfu2::JSONSerializer& stream)
	{
		stream.Deserialize( (bfu2::SerializableVector<SerializableClassInterface>*) &v_children );
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


	ComponentInterface* GameObject::AddComponent(const char* componentName)
	{
		return AddComponent( TypeInfo::GetTypeInfo( componentName )->id );
	}

	ComponentInterface* GameObject::AddComponent(size_t typeHash)
	{
		if( GetComponentOfTypeHash(typeHash) != nullptr)
			return nullptr;

		ComponentTranslatePointers copmponentInterfaces;

		ComponentInterface::AllocateAndInitObjectFromTypeHash(typeHash, m_mBlock, copmponentInterfaces);

		v_components.emplace_back( copmponentInterfaces );

		ComponentInterface* newComp = v_components.back().p_ComponentInterface;

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
			if(it->p_ComponentInterface==ptr)
			{
				v_components.erase(it);
				ptr->Detached();
				ptr->Dispouse();
				break;
			}
		}
	}
	ComponentInterface* GameObject::GetComponentOfTypeHash(size_t typeHash)
	{
		for(auto it = v_components.begin(); it!=v_components.end(); ++it)
		{
			if( it->p_ComponentInterface->TypeHash() == typeHash)
			{
				return it->p_ComponentInterface;
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
			v_components[i].p_ComponentInterface->OnGUI_NameAndVirtual();
		}
	}
	#endif
}