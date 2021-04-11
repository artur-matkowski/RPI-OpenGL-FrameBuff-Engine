#ifndef _H_GameObject
#define _H_GameObject
#include "EntityBase.hpp"
#include "_ComponentList.hpp"

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

	struct ComponentInfo: public EntityBase<ComponentInfo>
	{
		SERIALIZABLE_VAR( ComponentInfo, stream, m_componentTypeName );
		SERIALIZABLE_VAR( ComponentInfo, stream, m_recreationString );

		ComponentInfo( bfu::MemBlockBase* mBlock )
			:EntityBase(mBlock)
			,m_componentTypeName(mBlock)
			,m_recreationString(mBlock)
			{};
		ComponentInfo( const ComponentInfo& cp )
			:EntityBase(cp.m_mBlock)
			,m_componentTypeName(cp.m_mBlock)
			,m_recreationString(cp.m_mBlock)
			{ 
				m_componentTypeName = cp.m_componentTypeName; 
				m_recreationString = cp.m_recreationString;
			};
	};


	class GameObject: public EntityBase<GameObject>
	{
		friend bfu::ConditionalBuilder;
		

	protected:
		GameObject*										p_parent = 0;


		SERIALIZABLE_VAR( GameObject, string, m_myName );
		SERIALIZABLE_OBJ_VEC(GameObject, GameObject, v_children );
		// If you change name of v_children you need to change { if( loader!=0 && strcmp(it->first.c_str(), "v_children")==0 ) }

		bfu::ForwardAllocatorMemBlock forwardMemBlock;
		SERIALIZABLE_OBJ_VEC( GameObject, ComponentInfo, v_componentsInfo );

		std::vector<ComponentTranslatePointers, bfu::custom_allocator<ComponentTranslatePointers>> 
														v_components;

		Transform3D										*p_myTransform = nullptr;
		RendererComponent								*p_myRenderer = nullptr;
		PrefabLoaderComponent							*p_myPrefabLoader = nullptr;

		bool RegisterChild(GameObject* newChild);
		void UnRegisterChild(GameObject* deleteChild);

	public:
		GameObject( bfu::MemBlockBase* mBlock );
		GameObject( const GameObject& cp ) = delete;
		~GameObject();
		void ClearChildren();
		void ClearComponents(); 
		void AddChild();

		void RegisterTransform3D(Transform3D*);
		void RegisterRendererComponent(RendererComponent*);
		void RegisterPrefabLoaderComponent(PrefabLoaderComponent*);

		//TODO
		//GameObject& operator=(const GameObject& cp);

		virtual void PreDeserializationCallback()	override;
		virtual void PostDeserializationCallback()	override;
		virtual void PreSerializationCallback()		override;
		virtual void PostSerializationCallback()	override;

		void Init( bfu::MemBlockBase* mBlock );
		virtual void Dispouse();
		virtual void DispouseAndDeallocate();

		void SerializeChildren(bfu::JSONSerializer& stream);
		void DeserializeChildren(bfu::JSONSerializer& stream, PrefabMemBlock* prefabMemBlock);

		PROTECTED( void OnAttach(GameObject* newParrent) );
		void OnDetach();
		void ReAttach(GameObject* newParrent);

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
		inline ComponentTranslatePointers* GetComponent(int i)	
														{ return &v_components[i]; 	}

	};
}

#endif