#ifndef _H_ComponentBase
#define _H_ComponentBase
#include "ComponentInterface.hpp"
#include "Serializable.hpp"
#include <cxxabi.h>
#include <vector>

namespace asapi
{
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

		void Init( bfu::MemBlockBase* mBlock )
		{
			new (this) T(mBlock);
		};
		
		virtual size_t TypeHash() override
		{
			return typeid(T).hash_code();
		}

		virtual const char* TypeName()
		{
			return ClassName;
		}
	};

	template<class T>
	typename ComponentBase<T>::StaticInitializer ComponentBase<T>::initializer __attribute__((__used__));

	template<class T>
	char ComponentBase<T>::ClassName[255];



}

#endif