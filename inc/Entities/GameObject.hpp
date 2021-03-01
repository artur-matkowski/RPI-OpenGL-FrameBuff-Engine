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

	class GameObject: public EntityBase
	{
		friend bfu::ConditionalBuilder;

		struct ComponentInfo: public EntityBase
		{
			char buff[255];
			bfu::SerializableVar<bfu::stream>  			m_componentTypeName;
			bfu::SerializableVar<bfu::JSONStream>  		m_recreationString;

			ComponentInfo( bfu::MemBlockBase* mBlock )
				:EntityBase(mBlock)
				,m_componentTypeName("m_componentTypeName", this, buff, 255, mBlock)
				,m_recreationString("m_recreationString", this, mBlock)
				{};
			ComponentInfo( const ComponentInfo& cp )
				:EntityBase(cp.m_mBlock)
				,m_componentTypeName("m_componentTypeName", this, buff, 255, cp.m_mBlock)
				,m_recreationString("m_recreationString", this, cp.m_mBlock)
				{ 
					m_componentTypeName = cp.m_componentTypeName; 
					m_recreationString = cp.m_recreationString;
				};
		};

		void PopulateComponentInfo();
		void ClearComponentInfo();
		void ReconstructComponentsFromComponentInfo();
	protected:
		GameObject*										p_parent = 0;

		bfu::SerializableVar<bfu::string>  				m_myName;
		bfu::SerializableVarVector<GameObject*>			v_children;
		// If you change name of v_children you need to change { if( loader!=0 && strcmp(it->first.c_str(), "v_children")==0 ) }

		bfu::SerializableVarVector<ComponentInfo>		v_componentsInfo;

		std::vector<ComponentInterface*, bfu::custom_allocator<ComponentInterface*>> 
														v_components;
		Transform3D										*p_myTransform = 0;
		RendererComponent								*p_myRenderer = 0;

		virtual void RegisterChild(GameObject* newChild);
		virtual void UnRegisterChild(GameObject* deleteChild);

		GameObject(); //should never be used for prefabs. Is needed for template edduction in serialization
		GameObject( bfu::MemBlockBase* mBlock );

	public:
		GameObject( const GameObject& cp ) = delete;
		~GameObject();
		void ClearChildren();
		void ClearComponents(); 

		//TODO
		//GameObject& operator=(const GameObject& cp);

		void Init( bfu::MemBlockBase* mBlock );
		virtual void Dispouse();
		virtual void DispouseAndDeallocate();

		void OnLoad();
		void OnUnLoad();
		

		virtual void Serialize(bfu::JSONStream& stream);
		virtual void Deserialize(bfu::JSONStream& stream);

		void SerializeChildren(bfu::JSONStream& stream);
		void DeserializeChildren(bfu::JSONStream& stream);

		void OnAttach(GameObject* newParrent);
		void OnDetach();
		void ReAttach(GameObject* newParrent);

		ComponentInterface* AddComponent(size_t typeHash);
		ComponentInterface* AddComponent(const char* componentName);
		void RemoveComponent(ComponentInterface* ptr);
		ComponentInterface* GetComponentOfTypeHash(size_t typeHash);



		void SetName(const char*);
		inline const char* GetName()					{ return m_myName.c_str(); }

		inline int GetChildCount()						{ return v_children.size();	}
		inline GameObject* GetChild(int index)			{ return v_children[index]; }

		inline Transform3D* GetTransform3D()			{ return p_myTransform;	}
		inline GameObject* GetParent()					{ return p_parent; }

		#ifdef IS_EDITOR
		void OnGUI();
		#endif
	};
}

#endif