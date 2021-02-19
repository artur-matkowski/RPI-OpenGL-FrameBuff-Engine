#ifndef _H_ComponentBase
#define _H_ComponentBase
#include "EntityBase.hpp"
#include <unordered_map>
#include <cxxabi.h>
#include <vector>

namespace asapi
{


	class ComponentInterface;
	class GameObject;

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
		void OnGUInamed(const char* ComponentName);
		GameObject *m_owner = nullptr;
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

		virtual void OnAttach(){};
		virtual void OnDetach(){};
		virtual void OnGUI() = 0;
		virtual size_t TypeHash() = 0;
	};
	
	
	template<class T>
	static ComponentInterface* AllocateAndInit( bfu::MemBlockBase* mBlock )
	{
		T* obj = (T*) mBlock->allocate(1, sizeof(T), alignof(T) );
		obj->Init(mBlock);
		return obj;
	}


	template<class T>
	class ComponentBase: public ComponentInterface
	{
		static char ClassName[255];

		class StaticInitializer
		{
		public:
			StaticInitializer()
			{
				TypeInfo::RegisterType(AllocateAndInit<T>, typeid(T).hash_code(), sizeof(T), ClassName);

				size_t size = 255;
				int status;
 				abi::__cxa_demangle(typeid(T).name(), ClassName, &size, &status);

				log::error << "ComponentBase::StaticInitializer " << ClassName << std::endl;
			}
		};

		static  StaticInitializer initializer __attribute__((__used__)) ;
	protected:

	public:	
		ComponentBase(bfu::MemBlockBase* mBlock)
			:ComponentInterface(mBlock)
		{
		};
		~ComponentBase(){};

		
		virtual size_t TypeHash() override
		{
			return typeid(T).hash_code();
		}

		static const char* TypeName()
		{
			return ClassName;
		}

		virtual void OnGUI()
		{
			ComponentInterface::OnGUInamed( ClassName );
		}
	};

	template<class T>
	typename ComponentBase<T>::StaticInitializer ComponentBase<T>::initializer __attribute__((__used__));

	template<class T>
	char ComponentBase<T>::ClassName[255];



}

#endif