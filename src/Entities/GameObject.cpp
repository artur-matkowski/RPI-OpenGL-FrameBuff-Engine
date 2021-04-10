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
		,m_myName(mBlock)
		,forwardMemBlock(mBlock)
		,v_children(&forwardMemBlock)
		,v_componentsInfo(mBlock)
		,v_components(mBlock)
	{
		m_myName = "GameObject";

		AddComponent( typeid(asapi::Transform3D).hash_code() );
	}

	GameObject::~GameObject()
	{
		ClearComponents();
		ClearChildren();
	}
	void GameObject::ClearChildren()
	{
		v_children.clear();
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
				bfu::JSONSerializer serializer( std::move( v_componentsInfo[i].m_recreationString ) );
				serializer.Deserialize( v_components[i].p_SerializableClassInterface );
				v_componentsInfo[i].m_recreationString = std::move( serializer );

				v_components.back().p_ComponentInterface->OnIsDirty();
			}
		}
		v_componentsInfo.clear();

		for(int i=0; i<v_children.size(); ++i)
		{
			v_children[i].p_parent = this;
		}
	}
	void GameObject::PreSerializationCallback()
	{
		v_componentsInfo.clear();

		for(int i=0; i<v_components.size(); ++i)
		{
			ComponentInfo* obj = (ComponentInfo*) ComponentInfo::AllocateAndInit( SYSTEMS::JSON_ALLOCATOR );
			v_componentsInfo.push_back( obj );

			obj->m_componentTypeName.sprintf( v_components[i].p_ComponentInterface->TypeName() );

			bfu::JSONSerializer serializer( std::move( obj->m_recreationString ) );
			serializer.Serialize( v_components[i].p_SerializableClassInterface );
			obj->m_recreationString = std::move( serializer );
		}
	}
	void GameObject::PostSerializationCallback()
	{
		v_componentsInfo.clear();
	}


	void GameObject::SerializeChildren(bfu::JSONSerializer& stream)
	{
		stream.Serialize( (bfu::SerializableVector<SerializableClassInterface>*) &v_children );
	}
	void GameObject::DeserializeChildren(bfu::JSONSerializer& stream, PrefabMemBlock* prefabMemBlock)
	{
		forwardMemBlock = prefabMemBlock;

		stream.Deserialize( (bfu::SerializableVector<SerializableClassInterface>*) &v_children );

		for(int i=0; i<v_children.size(); ++i)
		{
			v_children[i].p_parent = this;
		}
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