#ifndef H_ComponentInterface
#define H_ComponentInterface
#include "EntityBase.hpp"
#include "SerializableRenderer.hpp"

namespace asapi
{
	class GameObject;
	class ComponentInterface;

	typedef ComponentInterface* (*InitFuncPtr)(bfu::MemBlockBase*);

	#define TYPE_INFO_CAPACITY 1024
	struct TypeInfo
	{
		InitFuncPtr 		fPtr;
		size_t 				id;
		size_t 				sizeOf;
		const char* 		name;

		static void RegisterType(InitFuncPtr, size_t, size_t, const char*);
		static TypeInfo* GetTypeInfo(size_t);
		static TypeInfo* GetTypeInfo(const char*);
		static TypeInfo* GetTypeInfo();
		static int GetTypeInfoSize();
	};


	class ComponentInterface: public EntityBase
	{
	protected:
		GameObject *m_owner = nullptr;
		#ifdef IS_EDITOR
		std::vector<SerializableRendererBase*>
					v_SerializableRenderers;
		#endif
	public:	
		ComponentInterface(bfu::MemBlockBase* mBlock)
			:EntityBase(mBlock)
		{};
		~ComponentInterface(){};
		
		void Attached(GameObject* owner);
		void Detached();

		static ComponentInterface* AllocateAndInitObjectFromTypeHash(size_t hash, bfu::MemBlockBase* mBlock)
		{
			return TypeInfo::GetTypeInfo(hash)->fPtr(mBlock);
		}

		virtual void PushReferenceToMap(const char* memberName, SerializableBase* memberReference)
		{
			bfu::SerializableClassBase::PushReferenceToMap(memberName, memberReference);
			//TODO add serializablefields to vector for easier rendering
		}

		#ifdef IS_EDITOR
		void PushSerializableRenderer(SerializableRendererBase*);
		void OnGUI_NameAndVirtual();
		virtual void OnGUI();
		#endif


		virtual void OnAttach(){};
		virtual void OnDetach(){};
		virtual size_t TypeHash() = 0;

		virtual const char* TypeName()
		{
			return "ComponentInterface";
		}

	};
}


#endif