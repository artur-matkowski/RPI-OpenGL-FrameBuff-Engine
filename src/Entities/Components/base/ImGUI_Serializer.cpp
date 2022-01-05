#include "ImGUI_Serializer.hpp"
#ifdef IS_EDITOR
#include "imgui.h"
#endif

namespace asapi
{


#define GENERATE_VECTOR_SERIALIZE_BODY(T) \
	ARGS* args = (ARGS*)data;\
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
	ARGS* args = (ARGS*)data;\
	T* _data = (T*)args->dataPtr;\
	T buff;\
	\
	buff = *_data;\



		void ImGUI_Serializer::Serialize( bfu::SerializableClassInterface* data )
		{
			bfu::ClassInfo* it = data->GetFirstClassInfo();
			ARGS args;

			args.data = data;


			for(; it != nullptr; )
			{
				args.it = it;
				args.dataPtr = (void*) ((size_t)data + it->offset);
				args.name = it->name;
				char tag[512];

				snprintf(tag, 512, "tableTag##%s", it->name);

				if( it->jsonSerializeFunc==bfu::SerializerBase::Serialize_SerializableClassInterface)
				{
					if (ImGui::BeginTable(tag, 1, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders))
					{
						ImGui::TableSetupColumn(it->name, ImGuiTableColumnFlags_WidthStretch);
						ImGui::TableHeadersRow();
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						it->jsonSerializeFunc(this, (void*) ((size_t)data + it->offset) );							
						
						ImGui::EndTable();
					}
				}
				else
				{
					it->jsonSerializeFunc(this, (void*) &args );
				}


				it = it->next;

				if( it!=nullptr )
				{
					ImGui::Spacing();
				}
			}
		}

		void ImGUI_Serializer::Serialize( bfu::SerializableVector<bfu::SerializableClassInterface>* data )
		{
		}

		void ImGUI_Serializer::Serialize( float* data )
		{
			GENERATE_SERIALIZE_BODY(float);

			if( ImGui::InputScalar(args->name, ImGuiDataType_Float, &buff) )
			{
				*_data = buff;
			}
		}

		void ImGUI_Serializer::Serialize( bfu::SerializableVector<float>* data )
		{
        	GENERATE_VECTOR_SERIALIZE_BODY(float);	
		}

		void ImGUI_Serializer::Serialize( bool* data )
		{
			GENERATE_SERIALIZE_BODY(bool);

			if( ImGui::Checkbox(args->name, &buff) )
			{
				*_data = buff;
			}
		}

		void ImGUI_Serializer::Serialize( bfu::SerializableVector<bool>* data )
		{
			ARGS* args = (ARGS*)data;
			bfu::SerializableVector<bool>* _data = (bfu::SerializableVector<bool>*)args->dataPtr;
			
			uint32_t size = _data->size();
			uint32_t step = 1;
			int i = 0;
			
			ImGui::Text("Vector size:");
			
			if( ImGui::InputScalar(args->it->name, ImGuiDataType_U32, &size, &step, NULL, "%d") )
			{
				_data->resize( size );
			}
			
			for(auto it = _data->begin(); it!=_data->end(); it++)
			{
				char name[512];
				snprintf(name, 512, "%s[%d]", args->it->name, i);
				args->name = name;
				bool tmp = (*_data)[i];		
				args->dataPtr = &tmp;	
				bool* __data = (bool*)args;	
				this->Serialize( __data );
				(*_data)[i] = tmp;
			
				++i;
			}	
		}

		void ImGUI_Serializer::Serialize( bfu::stream* data )
		{
			ARGS* args = (ARGS*)data;
			bfu::stream* _data = (bfu::stream*)args->dataPtr;
			char buff[512];

			strncpy(buff, _data->c_str(), 512);

			if( ImGui::InputText(args->name, buff, 512) )
			{
				*_data = buff;
			}
		}

		void ImGUI_Serializer::Serialize( bfu::SerializableVector<bfu::stream>* data )
		{
        	GENERATE_VECTOR_SERIALIZE_BODY(bfu::stream);			
		}

		void ImGUI_Serializer::Serialize( bfu::string* data )
		{
			ARGS* args = (ARGS*)data;
			bfu::string* _data = (bfu::string*)args->dataPtr;
			char buff[512] = {0};

			strncpy(buff, _data->c_str(), 512);

			if( ImGui::InputText(args->name, buff, 512) )
			{
				*_data = buff;
			}
		}

		void ImGUI_Serializer::Serialize( bfu::SerializableVector<bfu::string>* data )
		{
        	GENERATE_VECTOR_SERIALIZE_BODY(bfu::string);
		}

		void ImGUI_Serializer::Serialize( uint8_t* data )
		{

		}

		void ImGUI_Serializer::Serialize( bfu::SerializableVector<uint8_t>* data )
		{
        	GENERATE_VECTOR_SERIALIZE_BODY(uint8_t);			
		}

		void ImGUI_Serializer::Serialize( uint16_t* data )
		{

		}

		void ImGUI_Serializer::Serialize( bfu::SerializableVector<uint16_t>* data )
		{
        	GENERATE_VECTOR_SERIALIZE_BODY(uint16_t);			
		}

		void ImGUI_Serializer::Serialize( uint32_t* data )
		{

		}

		void ImGUI_Serializer::Serialize( bfu::SerializableVector<uint32_t>* data )
		{
        	GENERATE_VECTOR_SERIALIZE_BODY(uint32_t);			
		}

		void ImGUI_Serializer::Serialize( uint64_t* data )
		{
			GENERATE_SERIALIZE_BODY(uint64_t);

			if( ImGui::InputScalar(args->name, ImGuiDataType_U64, &buff) )
			{
				*_data = buff;
			}
		}

		void ImGUI_Serializer::Serialize( bfu::SerializableVector<uint64_t>* data )
		{
        	GENERATE_VECTOR_SERIALIZE_BODY(uint64_t);			
		}

		void ImGUI_Serializer::Serialize( int8_t* data )
		{

		}

		void ImGUI_Serializer::Serialize( bfu::SerializableVector<int8_t>* data )
		{
        	GENERATE_VECTOR_SERIALIZE_BODY(int8_t);			
		}

		void ImGUI_Serializer::Serialize( int16_t* data )
		{

		}

		void ImGUI_Serializer::Serialize( bfu::SerializableVector<int16_t>* data )
		{
        	GENERATE_VECTOR_SERIALIZE_BODY(int16_t);			
		}

		void ImGUI_Serializer::Serialize( int32_t* data )
		{

		}

		void ImGUI_Serializer::Serialize( bfu::SerializableVector<int32_t>* data )
		{
        	GENERATE_VECTOR_SERIALIZE_BODY(int32_t);			
		}

		void ImGUI_Serializer::Serialize( int64_t* data )
		{
			GENERATE_SERIALIZE_BODY(uint64_t);

			if( ImGui::InputScalar(args->name, ImGuiDataType_S64, &buff) )
			{
				*_data = buff;
			}
		}

		void ImGUI_Serializer::Serialize( bfu::SerializableVector<int64_t>* data )
		{
        	GENERATE_VECTOR_SERIALIZE_BODY(int64_t);			
		}




}