#include "ImGuiSerializer.hpp"
#include "SerializableObject.hpp"
#ifndef SERIALIZATIO_NOBJECT_TESTS
#include "imgui.h"
#endif

#define GENERATE_VECTOR_SERIALIZE_BODY(T) \
	ARGS_new* args = (ARGS_new*)data;\
	bfu::SerializableVector<T>* _data = (bfu::SerializableVector<T>*)args->dataPtr;\
	\
	uint32_t size = _data->size();\
	uint32_t step = 1;\
	int i = 0;\
	\
	ImGui::Text("Vector size:");\
	\
	if( ImGui::InputScalar(args->it->name, ImGuiDataType_U32, &size, &step, NULL, "%d") )\
	{\
		_data->resize( size );\
	}\
	\
	for(auto it = _data->begin(); it!=_data->end(); it++)\
	{\
		char name[512];\
		snprintf(name, 512, "%s[%d]", args->it->name, i);\
		args->name = name;\
		args->dataPtr = &*it;\
		T* __data = (T*)args;				\
		this->Serialize( __data );\
	\
		++i;\
	}\

#define GENERATE_SERIALIZE_BODY(T)\
	ARGS_new* args = (ARGS_new*)data;\
	T* _data = (T*)args->dataPtr;\
	T buff;\
	\
	buff = *_data;\


namespace asapi
{
	char* ImGUISerializer::data()
	{
		return (char*)nullptr;
	}

	int ImGUISerializer::size()
	{
		return 0;
	}

	void ImGUISerializer::clear()
	{
		return;
	}

	void ImGUISerializer::assignData(char* data, uint32_t size)
	{
		return;
	}











	void ImGUISerializer::Serialize( bfu::SerializableClassInterface* data )
	{
		ARGS_new* args = (ARGS_new*)data;
		bfu::SerializableClassInterface* _data = (bfu::SerializableClassInterface*)args->dataPtr;

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf( "\n%s:", args->name );

		#else

		#endif
	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<bfu::SerializableClassInterface>* data )
	{
		ARGS_new* args = (ARGS_new*)data;
		bfu::SerializableClassInterface* _data = (bfu::SerializableClassInterface*)args->dataPtr;

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf( "\n%s:", args->name );

		#else

		#endif
	}

	void ImGUISerializer::Serialize( float* data )
	{
		GENERATE_SERIALIZE_BODY(float);

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf("\n%s: %f", args->name, buff);

		#else

			if( ImGui::InputScalar(args->name, ImGuiDataType_Float, &buff) )
			{
				*_data = buff;
			}

		#endif
	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<float>* data )
	{

	}

	void ImGUISerializer::Serialize( bool* data )
	{

	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<bool>* data )
	{

	}

	void ImGUISerializer::Serialize( bfu::stream* data )
	{

	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<bfu::stream>* data )
	{

	}

	void ImGUISerializer::Serialize( bfu::string* data )
	{

	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<bfu::string>* data )
	{

	}

	
	void ImGUISerializer::Serialize( uint8_t* data )
	{

	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<uint8_t>* data )
	{

	}

	void ImGUISerializer::Serialize( uint16_t* data )
	{

	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<uint16_t>* data )
	{

	}

	void ImGUISerializer::Serialize( uint32_t* data )
	{

	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<uint32_t>* data )
	{

	}

	void ImGUISerializer::Serialize( uint64_t* data )
	{

	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<uint64_t>* data )
	{

	}


	void ImGUISerializer::Serialize( int8_t* data )
	{

	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<int8_t>* data )
	{

	}

	void ImGUISerializer::Serialize( int16_t* data )
	{

	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<int16_t>* data )
	{

	}

	void ImGUISerializer::Serialize( int32_t* data )
	{

	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<int32_t>* data )
	{

	}

	void ImGUISerializer::Serialize( int64_t* data )
	{

	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<int64_t>* data )
	{

	}
}