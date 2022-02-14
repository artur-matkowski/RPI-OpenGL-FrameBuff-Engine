#ifndef H_SerializableObject
#define H_SerializableObject
#include "bfu.hpp"
#include "object.hpp"
#include "ImGuiSerializer.hpp"

namespace asapi
{
	class SerializableObjectBase: public object
	{
	#ifdef IS_EDITOR
	protected:
		static void NextImGUIVecRow(const int column);
		static void NextImGUIRow();
	public:

		virtual void OnGUI(){}

		virtual void OnGUI_caller() = 0;
	#endif
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
				SerializableObjectBase* serializableObjectBase;
				//(((size_t)raw) + it->offset) + 8;// + (((size_t)serializableClassInterface) - ((size_t)raw));
				serializableObjectBase = dynamic_cast<SerializableObjectBase*>(this);
				serializableObjectBase = (SerializableObjectBase* ) (((size_t)serializableObjectBase) + it->offset);
				
				NextImGUIRow();
				serializableObjectBase->OnGUI_caller();

				it->jsonDeserializeFunc(&serializer, (void*) &args );
			}
			else if( it->jsonSerializeFunc == bfu::SerializerBase::Serialize_v_SerializableClassInterface )
			{
				CRTP* p_obj = dynamic_cast<CRTP*>(this);

				bfu::SerializableVector<bfu::SerializableClassInterface>* serializableObjectBaseVector 
					= //dynamic_cast<bfu::SerializableVector<bfu::SerializableClassInterface>*>( (void*) 
					(bfu::SerializableVector<bfu::SerializableClassInterface>*) (((size_t)p_obj) + it->offset);


				if( serializableObjectBaseVector->begin() != serializableObjectBaseVector->end() ) 
				{
					int i=0;
					for(auto vec_it = serializableObjectBaseVector->begin(); ; ) 
					{
						SerializableObjectBase* serializableObjectBase = nullptr;// = (SerializableObjectBase*) (*vec_it);
						//some pointer magic to get to corect vtable
						// = (SerializableObjectBase*) (((size_t)serializableObjectBase) + sizeof(void*));
						
						serializableObjectBase = dynamic_cast<SerializableObjectBase*>(*vec_it);
						//serializableObjectBase = (SerializableObjectBase* ) (((size_t)serializableObjectBase) + vec_it->offset);
						
						NextImGUIVecRow(i); ++i;
						serializableObjectBase->OnGUI_caller();
		 		 
						++vec_it; 
		 		 
						if( vec_it != serializableObjectBaseVector->end() ) 
						{ 
							//printf(", "); 
						} 
						else 
						{ 
							break; 
						} 
					} 
				} 
				it->jsonDeserializeFunc(&serializer, (void*) &args );
			}

			it = it->next;
		}

		serializableClassInterface->PostSerializationCallback();
	}
}

#endif