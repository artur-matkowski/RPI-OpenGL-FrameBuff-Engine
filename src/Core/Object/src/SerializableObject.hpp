#ifndef H_SerializableObject
#define H_SerializableObject
#include "bfu.hpp"
#include "object.hpp"
#include "ImGuiSerializer.hpp"

namespace asapi
{
	class SerializableObjectBase: public object
	{
	public:

		virtual void OnGUI(){}

		virtual void OnGUI_caller() = 0;
	};

	template<class CRTP>
	class SerializableObject: public bfu::SerializableClassBase<CRTP>, public SerializableObjectBase
	{
	public:
		virtual void OnGUI_caller() override;
	};

	



















	template<class CRTP>
	void SerializableObject<CRTP>::OnGUI_caller()
	{
		ImGUISerializer 					serializer;
		bfu::SerializableClassInterface* 	serializableClassInterface = this;
		void* 								raw = this;
		SerializableObjectBase* 			serializableObjectBase = this;


		if( &CRTP::OnGUI != &SerializableObjectBase::OnGUI )
		{
			serializableObjectBase->OnGUI();
			return;
		}


		serializableClassInterface->PreSerializationCallback();
		bfu::ClassInfo* it = serializableClassInterface->GetFirstClassInfo();

		for(; it != nullptr; )
		{
			ARGS_new args;

			args.data = (bfu::SerializableClassInterface*) ((size_t)raw + it->offset);
			args.it = it;
			args.dataPtr = (void*) ((size_t)raw + it->offset);
			args.name = it->name;

			it->jsonSerializeFunc(&serializer, (void*) &args );

			if( it->jsonSerializeFunc == bfu::SerializerBase::Serialize_SerializableClassInterface )
			{
				SerializableObjectBase* serializableObjectBase = (SerializableObjectBase*) (void*) 
				((size_t)raw + it->offset + ((size_t)serializableObjectBase - (size_t)raw) );
				
				serializableObjectBase->OnGUI_caller();
			}
			else if( it->jsonSerializeFunc == bfu::SerializerBase::Serialize_v_SerializableClassInterface )
			{
				bfu::SerializableVector<bfu::SerializableClassInterface>* serializableObjectBaseVector = (bfu::SerializableVector<bfu::SerializableClassInterface>*) (void*) 
				((size_t)raw + it->offset);// + ((size_t)serializableObjectBase - (size_t)raw) );

					//printf("size:%d", serializableObjectBaseVector->size() );

				if( serializableObjectBaseVector->begin() != serializableObjectBaseVector->end() ) 
				{
					for(auto it = serializableObjectBaseVector->begin(); ; ) 
					{
						SerializableObjectBase* serializableObjectBase = (SerializableObjectBase*) (*it);
						//some pointer magic to get to corect vtable
						serializableObjectBase = (SerializableObjectBase*) (((size_t)serializableObjectBase) + sizeof(void*));
						serializableObjectBase->OnGUI_caller(); 
		 		 
						++it; 
		 		 
						if( it != serializableObjectBaseVector->end() ) 
						{ 
							printf(", "); 
						} 
						else 
						{ 
							break; 
						} 
					} 
				} 
			}

			it = it->next;
		}

		serializableClassInterface->PostSerializationCallback();
	}
}

#endif