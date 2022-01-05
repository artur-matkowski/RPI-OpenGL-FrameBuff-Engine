#ifndef _H_ComponentBase
#define _H_ComponentBase
#include "ComponentInterface.hpp"
#include <cxxabi.h>
#include <vector>
#include "bfu.hpp"
#include "ImGUI_Serializer.hpp"

namespace asapi
{
#define SERIALIZABLE_GUI_OBJ(C, T, i) \
	T i; \
	static inline void initVar_##i() __attribute__((constructor, used)) \
	{ \
		static bool isRegistered = false; \
		if( isRegistered==false ) \
		{ \
			/*printf("%s offset: %d in class %s hash: %zu\n", #i, offsetOf(&C::i), #C, typeid(T).hash_code());*/ \
			FeedInfo(#i, offsetOf(&C::i), typeid(T).hash_code(), &C::sp_first, \
			OnGUI_caller_##i, \
			bfu::SerializerBase::Deserialize_SerializableClassInterface); \
			isRegistered = true; \
		} \
	} \
	static inline void OnGUI_caller_##i(bfu::SerializerBase* serializer, void* data) \
	{ \
		if( dynamic_cast<ImGUI_Serializer*>(serializer) != nullptr ) \
		{ \
			ARGS* args = (ARGS*)data;\
			T* _data = (T*)args->dataPtr;\
			_data->OnGUI_caller();\
		} \
		else \
		{ \
			bfu::SerializerBase::Serialize_SerializableClassInterface( serializer, data ); \
		} \
	} \


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

		
		#ifdef IS_EDITOR
		virtual void OnGUI()
		{
			ImGUI_Serializer imgui_serializer;

			imgui_serializer.Serialize( this );
		}
		#endif
	};

	template<class T>
	typename ComponentBase<T>::StaticInitializer ComponentBase<T>::initializer __attribute__((__used__));

	template<class T>
	char ComponentBase<T>::ClassName[255];



}

#endif