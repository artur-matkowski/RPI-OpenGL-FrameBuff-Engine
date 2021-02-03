#include "GameObject.hpp"

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


	void GameObject::Init( bfu::MemBlockBase* mBlock )
	{
		new (this) GameObject(mBlock);
	}
	void GameObject::OnLoad(GameObject* parrent)
	{
		p_parrent = parrent;
		parrent->RegisterChild( this );
	}
	void GameObject::OnUnLoad()
	{
		p_parrent->UnRegisterChild( this );
		p_parrent = 0;
	}



	void GameObject::SetName(const char* name)
	{
		m_myName = name;
	}

}