#ifndef _H_ComponentBase
#define _H_ComponentBase
#include "EntityBase.hpp"
#include <unordered_map>

namespace asapgl
{
	class ComponentInterface: public EntityBase
	{
		typedef ComponentInterface* (*InitFuncPtr)(bfu::MemBlockBase*);

		static std::unordered_map<size_t
						,InitFuncPtr
						,std::hash<size_t>
						,std::equal_to<size_t>
						,bfu::custom_allocator< std::pair<const size_t, InitFuncPtr> >
						> 
									s_componentAllocatorMap;


	public:	
		ComponentInterface(bfu::MemBlockBase* mBlock)
			:EntityBase(mBlock)
		{};
		~ComponentInterface(){};
		
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;

		static ComponentInterface* AllocateAndInitObjectFromTypeHash(size_t hash, bfu::MemBlockBase* mBlock)
		{
			return s_componentAllocatorMap[hash]( mBlock );
		}

		virtual void PushReferenceToMap(const char* memberName, SerializableBase* memberReference)
		{
			bfu::SerializableClassBase::PushReferenceToMap(memberName, memberReference);
			//TODO add serializablefields to vector for easier rendering
		}

		
		virtual void OnGUI()
		{
			//TODO render serializable fields in imgui
		}
	};


	template<class T>
	class ComponentBase: public ComponentInterface
	{
	protected:
		static ComponentInterface* AllocateAndInit( bfu::MemBlockBase* mBlock )
		{
			T* obj = (T*) mBlock->allocate(1, sizeof(T), alignof(T) );
			obj->Init(mBlock);
			return obj;
		}
	public:	
		ComponentBase(bfu::MemBlockBase* mBlock)
			:ComponentInterface(mBlock)
		{
			s_componentAllocatorMap[ TypeHash() ] = AllocateAndInit;
		};
		~ComponentBase(){};

		static size_t TypeHash()
		{
			return typeid(T).hash_code();
		}
	};
}

#endif