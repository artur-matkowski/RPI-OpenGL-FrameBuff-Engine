#ifndef _H_ComponentBase
#define _H_ComponentBase
#include "ComponentInterface.hpp"
#include <cxxabi.h>
#include <vector>
#include "bfu.hpp"

namespace asapi
{



	template<class T>
	static void AllocateAndInit( bfu::MemBlockBase* mBlock, ComponentTranslatePointers& ret )
	{
		T* obj = (T*) mBlock->allocate(1, sizeof(T), alignof(T) );
		obj->Init(mBlock);

		ret.p_ComponentInterface = obj;
		ret.p_SerializableClassInterface = obj;
	}


	template<class T>
	class ComponentBase: public bfu::SerializableClassBase<T>, public ComponentInterface
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
			}
		};

		static  StaticInitializer initializer __attribute__((__used__)) ;
	protected:

	public:	
		ComponentBase(bfu::MemBlockBase* mBlock /*= bfu::StdAllocatorMemBlock::GetMemBlock()*/ )
			:ComponentInterface(mBlock)
		{
		};

		ComponentBase(const ComponentBase&) = delete;
		~ComponentBase(){};

		void Init( bfu::MemBlockBase* mBlock )
		{
			new (this) T(mBlock);
		};
		
		virtual size_t TypeHash() override
		{
			return typeid(T).hash_code();
		}

		virtual const char* TypeName() override
		{
			return ClassName;
		}
		virtual void Dispouse() override
		{
			T* _obj = (T*)this;
			bfu::MemBlockBase::DeallocateUnknown(_obj);
		}
	};

	template<class T>
	typename ComponentBase<T>::StaticInitializer ComponentBase<T>::initializer __attribute__((__used__));

	template<class T>
	char ComponentBase<T>::ClassName[255];



}

#endif