#include "ImGuiSerializer.hpp"
#include "SerializableObject.hpp"

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
		delete (char*)nullptr; //code should never reach that callback
	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<bfu::SerializableClassInterface>* data )
	{

	}

	void ImGUISerializer::Serialize( float* data )
	{
		printf("%f", *data);
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



	void ImGUISerializer::Deserialize( bfu::SerializableClassInterface* data ) {}
	void ImGUISerializer::Deserialize( bfu::SerializableVector<bfu::SerializableClassInterface>* data ) {}
	void ImGUISerializer::Deserialize( float* data ) {}
	void ImGUISerializer::Deserialize( bfu::SerializableVector<float>* data ) {}
	void ImGUISerializer::Deserialize( bool* data ) {}
	void ImGUISerializer::Deserialize( bfu::SerializableVector<bool>* data ) {}
	void ImGUISerializer::Deserialize( bfu::stream* data ) {}
	void ImGUISerializer::Deserialize( bfu::SerializableVector<bfu::stream>* data ) {}
	void ImGUISerializer::Deserialize( bfu::string* data ) {}
	void ImGUISerializer::Deserialize( bfu::SerializableVector<bfu::string>* data ) {}

	void ImGUISerializer::Deserialize( uint8_t* data ) {}
	void ImGUISerializer::Deserialize( bfu::SerializableVector<uint8_t>* data ) {}
	void ImGUISerializer::Deserialize( uint16_t* data ) {}
	void ImGUISerializer::Deserialize( bfu::SerializableVector<uint16_t>* data ) {}
	void ImGUISerializer::Deserialize( uint32_t* data ) {}
	void ImGUISerializer::Deserialize( bfu::SerializableVector<uint32_t>* data ) {}
	void ImGUISerializer::Deserialize( uint64_t* data ) {}
	void ImGUISerializer::Deserialize( bfu::SerializableVector<uint64_t>* data ) {}

	void ImGUISerializer::Deserialize( int8_t* data ) {}
	void ImGUISerializer::Deserialize( bfu::SerializableVector<int8_t>* data ) {}
	void ImGUISerializer::Deserialize( int16_t* data ) {}
	void ImGUISerializer::Deserialize( bfu::SerializableVector<int16_t>* data ) {}
	void ImGUISerializer::Deserialize( int32_t* data ) {}
	void ImGUISerializer::Deserialize( bfu::SerializableVector<int32_t>* data ) {}
	void ImGUISerializer::Deserialize( int64_t* data ) {}
	void ImGUISerializer::Deserialize( bfu::SerializableVector<int64_t>* data ) {}
}