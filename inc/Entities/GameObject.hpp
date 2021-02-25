#ifndef _H_GameObject
#define _H_GameObject
#include "EntityBase.hpp"
#include "ComponentBase.hpp"

namespace asapi
{
	#define GAMEOBJECT_MAX_NAME_LENGTH 256
	#define SERIALIZATION_FILE_EXT ".json"
	#define SERIALIZATION_FOLDER "json/"

	class ComponentInterface;

	class GameObject: public EntityBase
	{
		friend bfu::ConditionalBuilder;

		struct ComponentInfo: public EntityBase
		{
			bfu::SerializableVar<size_t>  				m_typeId;
			bfu::SerializableVar<bfu::JSONStream>  		m_recreationString;

			ComponentInfo( bfu::MemBlockBase* mBlock )
				:EntityBase(mBlock)
				,m_typeId("m_typeId", this)
				,m_recreationString("m_recreationString", this, mBlock)
				{};
			ComponentInfo( const ComponentInfo& cp )
				:EntityBase(cp.m_mBlock)
				,m_typeId("m_typeId", this)
				,m_recreationString("m_recreationString", this, cp.m_mBlock)
				{ 
					m_typeId = cp.m_typeId; 
					m_recreationString = cp.m_recreationString;
				};
		};

		void PopulateComponentInfo();
		void ClearComponentInfo();
		void ReconstructComponentsFromComponentInfo();
	protected:
		bool 											b_isGameObjectLoader = false;
		GameObject*										p_parrent = 0;

		bfu::SerializableVar<bfu::string>  				m_myName;
		bfu::SerializableVarVector<GameObject*>			v_children;

		bfu::SerializableVarVector<ComponentInfo>		v_componentsInfo;

		std::vector<ComponentInterface*, bfu::custom_allocator<ComponentInterface*>> 
														v_components;

		virtual void RegisterChild(GameObject* newChild);
		virtual void UnRegisterChild(GameObject* deleteChild);

		GameObject(); //should never be used for prefabs. Is needed for template edduction in serialization
		GameObject( bfu::MemBlockBase* mBlock );

	public:
		GameObject( const GameObject& cp ) = delete;
		~GameObject();

		//TODO
		//GameObject& operator=(const GameObject& cp);

		void Init( bfu::MemBlockBase* mBlock );
		virtual void Dispouse();
		virtual void DispouseAndDeallocate();

		void OnLoad();
		void OnUnLoad();

		virtual void Serialize();
		virtual void Deserialize();

		virtual void Serialize(bfu::JSONStream& stream);
		virtual void Deserialize(bfu::JSONStream& stream);

		void OnAttach(GameObject* newParrent);
		void OnDetach();
		void ReAttach(GameObject* newParrent);

		void AddComponent(size_t typeHash);
		void RemoveComponent(ComponentInterface* ptr);
		ComponentInterface* GetComponentOfTypeHash(size_t typeHash);



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

		#ifdef IS_EDITOR
		void OnGUI();
		#endif
	};
}

#endif