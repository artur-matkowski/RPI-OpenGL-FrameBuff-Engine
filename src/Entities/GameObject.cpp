#include "GameObject.hpp"
#include "Systems.hpp"

namespace asapgl
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
			,m_myName("m_myName", this, mBlock)
			,v_children("v_children", this, mBlock)
	{
		m_myName.resize(GAMEOBJECT_MAX_NAME_LENGTH, '\0');
		m_myName = "GameObject";
	}

	GameObject::~GameObject()
	{

	}


	void GameObject::Init( bfu::MemBlockBase* mBlock )
	{
		new (this) GameObject(mBlock);
	}
	void GameObject::Dispouse()
	{
		for(auto it = v_children.begin(); 
			it != v_children.end();
			++it)
		{
			(*it)->DispouseAndDeallocate();
		}
		this->~GameObject();
	}
	void GameObject::DispouseAndDeallocate()
	{
		for(auto it = v_children.begin(); 
			it != v_children.end();
			++it)
		{
			(*it)->DispouseAndDeallocate();
		}
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
		
	}
	void GameObject::Deserialize()
	{

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

}