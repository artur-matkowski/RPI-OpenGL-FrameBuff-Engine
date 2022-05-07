#ifndef _H_GameObject
#define _H_GameObject
#include "_ComponentList.hpp"
#include "SerializableObject.hpp"

namespace asapi
{
	#define GAMEOBJECT_MAX_NAME_LENGTH 256
	#define SERIALIZATION_FILE_EXT ".json"
	#define SERIALIZATION_FOLDER "json/"

	class ComponentInterface;

	#define GET_COMPONENT(x) GetComponentOfTypeHash(typeid(x).hash_code())
	#define ADD_COMPONENT(x) AddComponent(typeid(x).hash_code())

	using bfu::string;
	using bfu::stream;
	using bfu::JSONSerializer;

	class ComponentInfo: public SerializableObject<ComponentInfo>
	{
		bfu::MemBlockBase* m_mBlock;

	public:
		SERIALIZABLE_VAR( ComponentInfo, stream, m_componentTypeName );
		SERIALIZABLE_VAR( ComponentInfo, stream, m_recreationString );

		ComponentInfo( bfu::MemBlockBase* mBlock ):
			m_mBlock(mBlock),
			m_componentTypeName(mBlock),
			m_recreationString(mBlock)
			{};
		ComponentInfo( const ComponentInfo& cp ):
			m_mBlock(cp.m_mBlock),
			m_componentTypeName(cp.m_mBlock),
			m_recreationString(cp.m_mBlock)
			{ 
				m_componentTypeName = cp.m_componentTypeName; 
				m_recreationString = cp.m_recreationString;
			};
	};


	//class GameObject: public EntityBase<GameObject>
	class GameObject: public SerializableObject<GameObject>
	{
		friend bfu::ConditionalBuilder;

		bfu::MemBlockBase* m_mBlock;

	public:

		bfu::MemBlockBase* GetMemBlock()
		{
			return m_mBlock;
		}
		

	protected:
		GameObject*										p_parent = 0;


		SERIALIZABLE_VAR( GameObject, string, m_myName );
		SERIALIZABLE_OBJ_VEC(GameObject, GameObject, v_children );
		// If you change name of v_children you need to change { if( loader!=0 && strcmp(it->first.c_str(), "v_children")==0 ) }

		bfu::ForwardAllocatorMemBlock forwardMemBlock;
		SERIALIZABLE_OBJ_VEC( GameObject, ComponentInfo, v_componentsInfo );

		std::vector<ComponentInterface*, bfu::custom_allocator<ComponentInterface*>> 
														v_components;

		Transform3D										*p_myTransform = nullptr;

		bool RegisterChild(GameObject* newChild);
		void UnRegisterChild(GameObject* deleteChild);


		void OnAttach(GameObject* newParrent);
		void OnDetach();

	public:
		GameObject( bfu::MemBlockBase* mBlock );
		GameObject( const GameObject& cp ) = delete;
		~GameObject();
		void ClearChildren();
		void ClearComponents(); 
		void AddChild();

		void RegisterTransform3D(Transform3D* p)		{ p_myTransform = p; }


		virtual void PreDeserializationCallback()		override;
		virtual void PostDeserializationCallback()		override;
		virtual void PreSerializationCallback()			override;
		virtual void PostSerializationCallback()		override;

		void Init( bfu::MemBlockBase* mBlock );
		void Detach() { OnDetach(); }
		virtual void Dispouse();
		virtual void DispouseAndDeallocate();

		void SerializeChildren(bfu::JSONSerializer& stream);
		void DeserializeChildren(bfu::JSONSerializer& stream, bfu::MemBlockBase* prefabMemBlock);


		ComponentInterface* AddComponent(size_t typeHash);
		ComponentInterface* AddComponent(const char* componentName);
		void RemoveComponent(ComponentInterface* ptr);
		ComponentInterface* GetComponentOfTypeHash(size_t typeHash);


		PrefabLoaderComponent* GetRegionalPrefabLoader();



		void SetName(const char*);
		inline const char* GetName()					{ return m_myName.c_str(); 		}

		inline int GetChildCount()						{ return v_children.size();	}
		inline GameObject* GetChild(int index)			{ return &v_children[index];	}

		inline Transform3D* GetTransform3D()			{ return p_myTransform;			}
		inline GameObject* GetParent()					{ return p_parent; 				}

		inline int GetComponentsCount()					{ return v_components.size(); 	}
		inline ComponentInterface* GetComponent(int i)	
														{ return v_components[i]; 	}

		#ifdef IS_EDITOR
		virtual void OnGUI() override;
		#endif
	};
}

#endif