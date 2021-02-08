#ifndef _H_GameObject
#define _H_GameObject
#include "EntityBase.hpp"

namespace asapgl
{
	#define GAMEOBJECT_MAX_NAME_LENGTH 256
	#define SERIALIZATION_FILE_EXT ".json"
	#define SERIALIZATION_FOLDER "json/"

	class GameObject: public EntityBase
	{
	protected:
		bool 											b_isGameObjectLoader = false;
		GameObject*										p_parrent = 0;

		bfu::SerializableVar<bfu::string>  				m_myName;
		bfu::SerializableVarVector<GameObject*>			v_children;

		virtual void RegisterChild(GameObject* newChild);
		virtual void UnRegisterChild(GameObject* deleteChild);

		GameObject( bfu::MemBlockBase* mBlock );

	public:
		GameObject( const GameObject& cp );
		~GameObject();

		GameObject& operator=(const GameObject& cp);

		void Init( bfu::MemBlockBase* mBlock );
		virtual void Dispouse();
		virtual void DispouseAndDeallocate();

		void OnLoad();
		void OnUnLoad();

		virtual void Serialize();
		virtual void Deserialize();

		//virtual void Serialize(JSONStream& stream);
		//virtual void Deserialize(JSONStream& stream);

		void OnAttach(GameObject* newParrent);
		void OnDetach();
		void ReAttach(GameObject* newParrent);



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

		inline bool IsGameObjectLoader()
		{
			return b_isGameObjectLoader;
		}

		void OnGUI();
	};
}

#endif