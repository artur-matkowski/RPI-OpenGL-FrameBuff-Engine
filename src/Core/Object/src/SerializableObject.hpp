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

		virtual void OnGUI()
		{

		}

		virtual void OnGUI_caller() = 0;
	};

	template<class CRTP>
	class SerializableObject: public bfu::SerializableClassBase<CRTP>, public SerializableObjectBase
	{
	public:

		virtual void OnGUI_caller() override
		{

			PointerTranslator translator;
			ImGUISerializer serializer;

			translator.serializableClassInterface = this;
			translator.raw = this;
			translator.serializableObjectBase = this;


			if( &CRTP::OnGUI != &SerializableObjectBase::OnGUI )
			{
				translator.serializableObjectBase->OnGUI();
				return;
			}


			translator.serializableClassInterface->PreSerializationCallback();
			bfu::ClassInfo* it = translator.serializableClassInterface->GetFirstClassInfo();

			for(; it != nullptr; )
			{
				printf("\n\"%s\": ", it->name );

				
				if( it->jsonSerializeFunc == bfu::SerializerBase::Serialize_SerializableClassInterface )
				{
					SerializableObjectBase* serializableObjectBase = (SerializableObjectBase*) (void*) 
					((size_t)translator.raw + it->offset + ((size_t)translator.serializableObjectBase - (size_t)translator.raw) );
					
					serializableObjectBase->OnGUI_caller();
				}
				else if( it->jsonSerializeFunc == bfu::SerializerBase::Serialize_v_SerializableClassInterface )
				{
					bfu::SerializableVector<bfu::SerializableClassInterface>* serializableObjectBaseVector = (bfu::SerializableVector<bfu::SerializableClassInterface>*) (void*) 
					((size_t)translator.raw + it->offset);// + ((size_t)translator.serializableObjectBase - (size_t)translator.raw) );

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
				else 
				{
					it->jsonSerializeFunc(&serializer, (void*) ((size_t)translator.raw + it->offset) );
				}

				it = it->next;

				if( it != nullptr )
				{
					printf(", ");
				}
			}

			translator.serializableClassInterface->PostSerializationCallback();
		}
	};
}

#endif