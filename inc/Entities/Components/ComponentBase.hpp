#ifndef _H_ComponentBase
#define _H_ComponentBase
#include "EntityBase.hpp"
#include <unordered_map>
#include <cxxabi.h>
#include <vector>

namespace asapgl
{
	class ComponentInterface;

	typedef ComponentInterface* (*InitFuncPtr)(bfu::MemBlockBase*);

	struct TypeInfo
	{
		InitFuncPtr 		fPtr;
		const char* 		name;
	};

	static std::map<size_t
					,TypeInfo
					,std::less<size_t>
					,bfu::custom_allocator< std::pair<const size_t, TypeInfo> >
					> 
					s_componentAllocatorMap;


	class ComponentInterface: public EntityBase
	{
	protected:
		void OnGUInamed(const char* ComponentName);
	public:	
		ComponentInterface(bfu::MemBlockBase* mBlock)
			:EntityBase(mBlock)
		{};
		~ComponentInterface(){};
		
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;

		static ComponentInterface* AllocateAndInitObjectFromTypeHash(size_t hash, bfu::MemBlockBase* mBlock)
		{
			return s_componentAllocatorMap[hash].fPtr(mBlock);
		}

		virtual void PushReferenceToMap(const char* memberName, SerializableBase* memberReference)
		{
			bfu::SerializableClassBase::PushReferenceToMap(memberName, memberReference);
			//TODO add serializablefields to vector for easier rendering
		}

		virtual void OnGUI() = 0;
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
				s_componentAllocatorMap[ typeid(T).hash_code() ] = TypeInfo{AllocateAndInit<T>, ClassName};
				size_t size = 255;
				int status;
 				abi::__cxa_demangle(typeid(T).name(), ClassName, &size, &status);

				//log::error << "ComponentBase::StaticInitializer " << ComponentBase<T>::TypeHash() << " " << ClassName << std::endl;
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

		static size_t TypeHash()
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