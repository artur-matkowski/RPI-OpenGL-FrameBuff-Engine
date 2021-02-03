#ifndef _H_GameObject
#define _H_GameObject
#include "EntityBase.hpp"

namespace asapgl
{
	//class HierarchyWindow;

	#define GAMEOBJECT_MAX_NAME_LENGTH 256

	class GameObject: public EntityBase
	{
		//friend HierarchyWindow;		//needed for OnGUI not rendering root node
	protected:
		GameObject*										p_parrent = 0;
		//SERIALIZABLE_VAR_VEC( UniqueID ) 	m_usedComponentsIDs;
		//SERIALIZABLE_VAR_VEC( UniqueID )	m_usedGameObjectsChildrensIDs;
		//SERIALIZABLE_VAR( UniqueID )		m_usedGameObjectParrentID;
		bfu::SerializableVar<bfu::string>  				m_myName;
		bfu::SerializableVarVector<GameObject*>			v_children;

		void RegisterChild(GameObject* newChild);
		void UnRegisterChild(GameObject* deleteChild);

		GameObject( bfu::MemBlockBase* mBlock );
		//~GameObject();

	public:
		void Init( bfu::MemBlockBase* mBlock );

		void ReAttach(GameObject* newParrent);
		void OnLoad(GameObject* parrent);
		void OnUnLoad();


		void SetName(const char*);
		inline const char* GetName()
		{
			return m_myName.c_str();
		}

		inline int GetChildCount()
		{
			return v_children.size();
		}

		inline GameObject* GetChild(int index)
		{
			return v_children[index];
		}


	};
}

#endif