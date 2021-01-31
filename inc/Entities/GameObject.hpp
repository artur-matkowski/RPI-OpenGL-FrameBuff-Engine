#ifndef _H_GameObject
#define _H_GameObject
#include "EntityBase.hpp"

namespace asapgl
{
	class SceneSystem;
	class HierarchyWindow;

	class GameObject: public EntityBase
	{
		friend SceneSystem;
		friend HierarchyWindow;		//needed for OnGUI not rendering root node
	protected:
		GameObject*										p_parrent = 0;
		//SERIALIZABLE_VAR_VEC( UniqueID ) 	m_usedComponentsIDs;
		//SERIALIZABLE_VAR_VEC( UniqueID )	m_usedGameObjectsChildrensIDs;
		//SERIALIZABLE_VAR( UniqueID )		m_usedGameObjectParrentID;
		bfu::SerializableVar<bfu::string>  				m_myName;
		bfu::SerializableVarVector<GameObject*>			v_children;

		void RegisterChild(GameObject* newChild);
		void UnRegisterChild(GameObject* deleteChild);
	public:
		GameObject( bfu::MemBlockBase* mBlock )
			:EntityBase(mBlock)
			,m_myName("m_myName", this, mBlock)
			,v_children("v_children", this, mBlock)
		{
			m_myName = "GameObject";
		}

		void OnLoad(GameObject* parrent);
		void OnUnLoad();

		void SetName(const char*);

	};
}

#endif