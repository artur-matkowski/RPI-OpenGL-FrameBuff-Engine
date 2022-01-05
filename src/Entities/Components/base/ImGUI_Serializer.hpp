#ifndef H_ImGUI_Serializer
#define H_ImGUI_Serializer

#include "bfu.hpp"

namespace asapi
{
	struct ARGS
	{
		bfu::SerializableClassInterface* 	data = nullptr;
		bfu::ClassInfo* 					it = nullptr;
		void* 								dataPtr = nullptr;
		const char*							name = nullptr;
	};

	class ImGUI_Serializer: public bfu::SerializerBase
	{
	public:
		virtual char* data() override { return nullptr; };
		virtual int size() override { return -1; };
		virtual void clear() override {};
		virtual void assignData(char* data, uint32_t size) override {};


		ImGUI_Serializer(){};

		virtual void SERIALIZER_SECTION Serialize( bfu::SerializableClassInterface* data ) override;
		virtual void SERIALIZER_SECTION Serialize( bfu::SerializableVector<bfu::SerializableClassInterface>* data ) override;
		virtual void SERIALIZER_SECTION Serialize( float* data ) override;
		virtual void SERIALIZER_SECTION Serialize( bfu::SerializableVector<float>* data ) override;
		virtual void SERIALIZER_SECTION Serialize( bool* data ) override;
		virtual void SERIALIZER_SECTION Serialize( bfu::SerializableVector<bool>* data ) override;
		virtual void SERIALIZER_SECTION Serialize( bfu::stream* data ) override;
		virtual void SERIALIZER_SECTION Serialize( bfu::SerializableVector<bfu::stream>* data ) override;
		virtual void SERIALIZER_SECTION Serialize( bfu::string* data ) override;
		virtual void SERIALIZER_SECTION Serialize( bfu::SerializableVector<bfu::string>* data ) override;
		
		virtual void SERIALIZER_SECTION Serialize( uint8_t* data ) override;
		virtual void SERIALIZER_SECTION Serialize( bfu::SerializableVector<uint8_t>* data ) override;
		virtual void SERIALIZER_SECTION Serialize( uint16_t* data ) override;
		virtual void SERIALIZER_SECTION Serialize( bfu::SerializableVector<uint16_t>* data ) override;
		virtual void SERIALIZER_SECTION Serialize( uint32_t* data ) override;
		virtual void SERIALIZER_SECTION Serialize( bfu::SerializableVector<uint32_t>* data ) override;
		virtual void SERIALIZER_SECTION Serialize( uint64_t* data ) override;
		virtual void SERIALIZER_SECTION Serialize( bfu::SerializableVector<uint64_t>* data ) override;

		virtual void SERIALIZER_SECTION Serialize( int8_t* data ) override;
		virtual void SERIALIZER_SECTION Serialize( bfu::SerializableVector<int8_t>* data ) override;
		virtual void SERIALIZER_SECTION Serialize( int16_t* data ) override;
		virtual void SERIALIZER_SECTION Serialize( bfu::SerializableVector<int16_t>* data ) override;
		virtual void SERIALIZER_SECTION Serialize( int32_t* data ) override;
		virtual void SERIALIZER_SECTION Serialize( bfu::SerializableVector<int32_t>* data ) override;
		virtual void SERIALIZER_SECTION Serialize( int64_t* data ) override;
		virtual void SERIALIZER_SECTION Serialize( bfu::SerializableVector<int64_t>* data ) override;





		//---------------




		virtual void DESERIALIZER_SECTION Deserialize( bfu::SerializableClassInterface* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( bfu::SerializableVector<bfu::SerializableClassInterface>* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( float* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( bfu::SerializableVector<float>* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( bool* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( bfu::SerializableVector<bool>* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( bfu::stream* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( bfu::SerializableVector<bfu::stream>* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( bfu::string* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( bfu::SerializableVector<bfu::string>* data ) override{};

		virtual void DESERIALIZER_SECTION Deserialize( uint8_t* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( bfu::SerializableVector<uint8_t>* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( uint16_t* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( bfu::SerializableVector<uint16_t>* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( uint32_t* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( bfu::SerializableVector<uint32_t>* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( uint64_t* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( bfu::SerializableVector<uint64_t>* data ) override{};

		virtual void DESERIALIZER_SECTION Deserialize( int8_t* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( bfu::SerializableVector<int8_t>* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( int16_t* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( bfu::SerializableVector<int16_t>* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( int32_t* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( bfu::SerializableVector<int32_t>* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( int64_t* data ) override{};
		virtual void DESERIALIZER_SECTION Deserialize( bfu::SerializableVector<int64_t>* data ) override{};
	};
}

#endif