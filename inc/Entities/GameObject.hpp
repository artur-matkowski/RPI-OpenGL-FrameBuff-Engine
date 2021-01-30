#ifndef _H_GameObject
#define _H_GameObject
#include "EntityBase.hpp"

namespace asapgl
{
	class GameObject: public EntityBase
	{
	protected:
		GameObject*										p_parrent = 0;
		//SERIALIZABLE_VAR_VEC( UniqueID ) 	m_usedComponentsIDs;
		//SERIALIZABLE_VAR_VEC( UniqueID )	m_usedGameObjectsChildrensIDs;
		//SERIALIZABLE_VAR( UniqueID )		m_usedGameObjectParrentID;
		bfu::SerializableVarVector<GameObject*>			v_children;
		bfu::SerializableVar<bfu::string>  				m_myName;

		void RegisterChild(GameObject* newChild);
		void UnRegisterChild(GameObject* deleteChild);
	public:
		GameObject( bfu::MemBlockBase* mBlock )
			:EntityBase(mBlock)
			,v_children("v_children", this, mBlock)
			,m_myName("m_myName", this, mBlock)
		{}

		void OnLoad(GameObject* parrent);
		void OnUnLoad();

		void OnGUI();

	};
}

#endif