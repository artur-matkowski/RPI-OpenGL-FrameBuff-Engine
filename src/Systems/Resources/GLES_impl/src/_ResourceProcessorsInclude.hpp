#include "ResourceTXTProcessor.hpp"
#include "ResourceFBXProcessor.hpp"
#include "ResourceGLSLProcessor.hpp"


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
	} 

