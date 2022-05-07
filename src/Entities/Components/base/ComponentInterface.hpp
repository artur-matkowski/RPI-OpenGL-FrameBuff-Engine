#ifndef H_ComponentInterface
#define H_ComponentInterface
#include "bfu.hpp"
#include "SerializableObject.hpp"
#include "UniqueID.hpp"

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


	class ComponentInterface
	{
	protected:
		GameObject *m_owner = nullptr;
	public:	
		ComponentInterface(bfu::MemBlockBase* mBlock);
		~ComponentInterface(){};
		
		void Attached(GameObject* owner);
		void Detached();

		static ComponentInterface* AllocateAndInitObjectFromTypeHash(size_t hash, bfu::MemBlockBase* mBlock)
		{
			return TypeInfo::GetTypeInfo(hash)->fPtr(mBlock);
		}

		virtual void OnAttach(){};
		virtual void OnDetach(){};
		virtual void OnIsDirty(){};
		virtual size_t TypeHash() = 0;
		virtual void Dispouse() = 0;

		virtual const char* TypeName()
		{
			return "ComponentInterface";
		}

		inline GameObject* GetOwner()		{ return m_owner; 		}
	};
}


#endif