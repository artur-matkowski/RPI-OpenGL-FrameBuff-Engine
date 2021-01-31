#include "GameObject.hpp"

namespace asapgl
{

	void GameObject::OnLoad(GameObject* parrent)
	{
		p_parrent = parrent;
		parrent->RegisterChild( this );
	}
	void GameObject::OnUnLoad()
	{
		p_parrent->UnRegisterChild( this );
	}


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


	void GameObject::SetName(const char* name)
	{
		m_myName = name;
	}
}