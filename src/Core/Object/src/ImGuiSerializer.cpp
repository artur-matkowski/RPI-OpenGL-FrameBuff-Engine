#include "ImGuiSerializer.hpp"
#include "SerializableObject.hpp"
#ifndef SERIALIZATIO_NOBJECT_TESTS
#include "imgui.h"
#endif



#define GENERATE_SERIALIZE_BODY(T)\
	ARGS_new* args = (ARGS_new*)data;\
	T* _data = (T*)args->dataPtr;\
	char taggedName[128];\
	snprintf(taggedName,128,"%s##%llu", args->name, (size_t)_data);\
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

			char id[32];
			snprintf(id, 32, "%llu", (size_t)data);

       		ImGui::BeginTable(id, 1, ImGuiTableFlags_BordersOuter);
    
            ImGui::TableSetupColumn(args->name);
            ImGui::TableHeadersRow();

		#endif
	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<bfu::SerializableClassInterface>* data )
	{
		
		ARGS_new* args = (ARGS_new*)data;
		bfu::SerializableVector<bfu::SerializableClassInterface>* _data = 
			(bfu::SerializableVector<bfu::SerializableClassInterface>*)args->dataPtr;

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf( "\n%s:", args->name );

		#else
			
			uint32_t size = _data->size();
			uint32_t step = 1;
			char id[32];
			snprintf(id, 32, "%llu", (size_t)_data);


       		ImGui::BeginTable(id, 2, ImGuiTableFlags_Borders);
    
            ImGui::TableSetupColumn(args->name, ImGuiTableColumnFlags_WidthFixed, 100.0f);
            ImGui::TableSetupColumn("Vector size:", ImGuiTableColumnFlags_WidthStretch);

            ImGui::TableHeadersRow();
			
			if( ImGui::InputScalar(args->it->name, ImGuiDataType_U32, &size, &step, NULL, "%d") )
			{
				const int oldSize = _data->size();

				if(size > oldSize)
					_data->reserve( size );
				else if(size < oldSize)
					_data->resize( size );

				for(int i=oldSize; i<size; i++)
				{
					bfu::SerializableClassInterface* cache = _data->allocateAndInit( _data->mBlock() );
					_data->emplace_back( cache );
				}
			}


		#endif
		
	}


	void ImGUISerializer::Serialize( float* data )
	{
		GENERATE_SERIALIZE_BODY(float);

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf("\n%s: %f", args->name, buff);

		#else

			if( ImGui::InputScalar(taggedName, ImGuiDataType_Float, &buff) )
			{
				*_data = buff;
			}

		#endif
	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<float>* data )
	{
		ARGS_new* args = (ARGS_new*)data;
		bfu::SerializableVector<float>* _data = 
			(bfu::SerializableVector<float>*)args->dataPtr;

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf( "\n%s:", args->name );

		#else

			uint32_t size = _data->size();
			uint32_t step = 1;
			char id[32];
			snprintf(id, 32, "%s##%llu",  args->name, (size_t)data);


			if(ImGui::BeginTable(id, 1, ImGuiTableFlags_Borders))
			{
			    ImGui::TableSetupColumn("Vector size:", ImGuiTableColumnFlags_WidthStretch);

				ImGui::TableHeadersRow();
				
				if( ImGui::InputScalar(args->it->name, ImGuiDataType_U32, &size, &step, NULL, "%d") )
				{
					_data->resize( size );
				}

				for(int i=0; i<_data->size(); ++i)
				{
					float buff = (*_data)[i];
					char name[16];
					snprintf(name, 16, "%d", i);

			        ImGui::TableNextRow();
				    ImGui::TableSetColumnIndex( 0 );

					if( ImGui::InputScalar(name, ImGuiDataType_Float, &buff) )
					{
						(*_data)[i] = buff;
					}
				}
				
			    ImGui::EndTable();
			}

		#endif
	}

	void ImGUISerializer::Serialize( bool* data )
	{
		GENERATE_SERIALIZE_BODY(bool);

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf("\n%s: %f", args->name, buff);

		#else

			if( ImGui::Checkbox(taggedName, &buff) )
			{
				*_data = buff;
			}

		#endif
	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<bool>* data )
	{
		ARGS_new* args = (ARGS_new*)data;
		bfu::SerializableVector<bool>* _data = 
			(bfu::SerializableVector<bool>*)args->dataPtr;

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf( "\n%s:", args->name );

		#else

			uint32_t size = _data->size();
			uint32_t step = 1;
			char id[32];
			snprintf(id, 32, "%s##%llu",  args->name, (size_t)data);


			if(ImGui::BeginTable(id, 1, ImGuiTableFlags_Borders))
			{
			    ImGui::TableSetupColumn("Vector size:", ImGuiTableColumnFlags_WidthStretch);

				ImGui::TableHeadersRow();
				
				if( ImGui::InputScalar(args->it->name, ImGuiDataType_U32, &size, &step, NULL, "%d") )
				{
					_data->resize( size );
				}

				for(int i=0; i<_data->size(); ++i)
				{
					bool buff = (*_data)[i];
					char name[16];
					snprintf(name, 16, "%d", i);

			        ImGui::TableNextRow();
				    ImGui::TableSetColumnIndex( 0 );

					if( ImGui::Checkbox(name, &buff) )
					{
						(*_data)[i] = buff;
					}
				}
				
			    ImGui::EndTable();
			}

		#endif

	}

	void ImGUISerializer::Serialize( bfu::stream* data )
	{
		ARGS_new* args = (ARGS_new*)data;
		bfu::stream* _data = (bfu::stream*)args->dataPtr;
		char taggedName[128];
		snprintf(taggedName,128,"%s##%llu", args->name, (size_t)_data);

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf("\n%s: %s", args->name, _data->c_str());

		#else

			static int buffsize = 4;
			static char* buff = new char[buffsize];

			if( buffsize-2 < (*_data).capacity() )
			{
				buffsize = (*_data).capacity()*2;
				delete buff;
				buff = new char[buffsize];
			}

			strncpy(buff, (*_data).c_str(), buffsize);

			if( ImGui::InputText(taggedName, buff, buffsize ) )
			{
				const int bufflength = strlen(buff);
				if( bufflength > (*_data).capacity()-2 )
				{
					(*_data).grow(bufflength);
				}
				(*_data).clear();
				(*_data).sprintf( buff );
			}

		#endif
	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<bfu::stream>* data )
	{
		ARGS_new* args = (ARGS_new*)data;
		bfu::SerializableVector<bfu::stream>* _data = 
			(bfu::SerializableVector<bfu::stream>*)args->dataPtr;

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf( "\n%s:", args->name );

		#else

			uint32_t size = _data->size();
			uint32_t step = 1;
			char id[32];
			snprintf(id, 32, "%s##%llu",  args->name, (size_t)data);


			if(ImGui::BeginTable(id, 1, ImGuiTableFlags_Borders))
			{
			    ImGui::TableSetupColumn("Vector size:", ImGuiTableColumnFlags_WidthStretch);

				ImGui::TableHeadersRow();
				
				if( ImGui::InputScalar(args->it->name, ImGuiDataType_U32, &size, &step, NULL, "%d") )
				{
					_data->resize( size );
				}

				for(int i=0; i<_data->size(); ++i)
				{
					char name[16];
					snprintf(name, 16, "%d", i);

					static int buffsize = 4;
					static char* buff = new char[buffsize];

					if( buffsize-2 < (*_data)[i].capacity() )
					{
						buffsize = (*_data)[i].capacity()*2;
						delete buff;
						buff = new char[buffsize];
					}

					strncpy(buff, (*_data)[i].c_str(), buffsize);

			        ImGui::TableNextRow();
				    ImGui::TableSetColumnIndex( 0 );

            		if( ImGui::InputText(name, buff, buffsize ) )
            		{
            			const int bufflength = strlen(buff);
            			if( bufflength > (*_data)[i].capacity()-2 )
            			{
            				(*_data)[i].grow(bufflength);
            			}
            			(*_data)[i].clear();
            			(*_data)[i].sprintf( buff );
            		}
				}
				
			    ImGui::EndTable();
			}

		#endif

	}

	void ImGUISerializer::Serialize( bfu::string* data )
	{
		ARGS_new* args = (ARGS_new*)data;
		bfu::string* _data = (bfu::string*)args->dataPtr;
		char taggedName[128];
		snprintf(taggedName,128,"%s##%llu", args->name, (size_t)_data);

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf("\n%s: %s", args->name, _data->c_str());

		#else

			static int buffsize = 4;
			static char* buff = new char[buffsize];

			if( buffsize-2 < (*_data).capacity() )
			{
				buffsize = (*_data).capacity()*2;
				delete buff;
				buff = new char[buffsize];
			}

			strncpy(buff, (*_data).c_str(), buffsize);

			if( ImGui::InputText(taggedName, buff, buffsize ) )
			{
				(*_data) = buff;
			}

		#endif
	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<bfu::string>* data )
	{
		ARGS_new* args = (ARGS_new*)data;
		bfu::SerializableVector<bfu::string>* _data = 
			(bfu::SerializableVector<bfu::string>*)args->dataPtr;

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf( "\n%s:", args->name );

		#else

			uint32_t size = _data->size();
			uint32_t step = 1;
			char id[32];
			snprintf(id, 32, "%s##%llu",  args->name, (size_t)data);


			if(ImGui::BeginTable(id, 1, ImGuiTableFlags_Borders))
			{
			    ImGui::TableSetupColumn("Vector size:", ImGuiTableColumnFlags_WidthStretch);

				ImGui::TableHeadersRow();
				
				if( ImGui::InputScalar(args->it->name, ImGuiDataType_U32, &size, &step, NULL, "%d") )
				{
					_data->resize( size );
				}

				for(int i=0; i<_data->size(); ++i)
				{
					char name[16];
					snprintf(name, 16, "%d", i);

					static int buffsize = 4;
					static char* buff = new char[buffsize];

					if( buffsize-2 < (*_data)[i].capacity() )
					{
						buffsize = (*_data)[i].capacity()*2;
						delete buff;
						buff = new char[buffsize];
					}

					strncpy(buff, (*_data)[i].c_str(), buffsize);

			        ImGui::TableNextRow();
				    ImGui::TableSetColumnIndex( 0 );

            		if( ImGui::InputText(name, buff, buffsize ) )
            		{
            			(*_data)[i] = buff;
            		}
				}
				
			    ImGui::EndTable();
			}

		#endif
	}

	
	void ImGUISerializer::Serialize( uint8_t* data )
	{
		GENERATE_SERIALIZE_BODY(uint8_t);

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf("\n%s: %f", args->name, buff);

		#else

			if( ImGui::InputScalar(taggedName, ImGuiDataType_U8, &buff) )
			{
				*_data = buff;
			}

		#endif
	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<uint8_t>* data )
	{
		ARGS_new* args = (ARGS_new*)data;
		bfu::SerializableVector<uint8_t>* _data = 
			(bfu::SerializableVector<uint8_t>*)args->dataPtr;

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf( "\n%s:", args->name );

		#else

			uint32_t size = _data->size();
			uint32_t step = 1;
			char id[32];
			snprintf(id, 32, "%s##%llu",  args->name, (size_t)data);


			if(ImGui::BeginTable(id, 1, ImGuiTableFlags_Borders))
			{
			    ImGui::TableSetupColumn("Vector size:", ImGuiTableColumnFlags_WidthStretch);

				ImGui::TableHeadersRow();
				
				if( ImGui::InputScalar(args->it->name, ImGuiDataType_U32, &size, &step, NULL, "%d") )
				{
					_data->resize( size );
				}

				for(int i=0; i<_data->size(); ++i)
				{
					uint8_t buff = (*_data)[i];
					char name[16];
					snprintf(name, 16, "%d", i);

			        ImGui::TableNextRow();
				    ImGui::TableSetColumnIndex( 0 );

					if( ImGui::InputScalar(name, ImGuiDataType_U8, &buff) )
					{
						(*_data)[i] = buff;
					}
				}
				
			    ImGui::EndTable();
			}

		#endif
	}

	void ImGUISerializer::Serialize( uint16_t* data )
	{
		GENERATE_SERIALIZE_BODY(uint16_t);

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf("\n%s: %f", args->name, buff);

		#else

			if( ImGui::InputScalar(taggedName, ImGuiDataType_U16, &buff) )
			{
				*_data = buff;
			}

		#endif
	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<uint16_t>* data )
	{
		ARGS_new* args = (ARGS_new*)data;
		bfu::SerializableVector<uint16_t>* _data = 
			(bfu::SerializableVector<uint16_t>*)args->dataPtr;

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf( "\n%s:", args->name );

		#else

			uint32_t size = _data->size();
			uint32_t step = 1;
			char id[32];
			snprintf(id, 32, "%s##%llu",  args->name, (size_t)data);


			if(ImGui::BeginTable(id, 1, ImGuiTableFlags_Borders))
			{
			    ImGui::TableSetupColumn("Vector size:", ImGuiTableColumnFlags_WidthStretch);

				ImGui::TableHeadersRow();
				
				if( ImGui::InputScalar(args->it->name, ImGuiDataType_U32, &size, &step, NULL, "%d") )
				{
					_data->resize( size );
				}

				for(int i=0; i<_data->size(); ++i)
				{
					uint16_t buff = (*_data)[i];
					char name[16];
					snprintf(name, 16, "%d", i);

			        ImGui::TableNextRow();
				    ImGui::TableSetColumnIndex( 0 );

					if( ImGui::InputScalar(name, ImGuiDataType_U16, &buff) )
					{
						(*_data)[i] = buff;
					}
				}
				
			    ImGui::EndTable();
			}

		#endif
	}

	void ImGUISerializer::Serialize( uint32_t* data )
	{
		GENERATE_SERIALIZE_BODY(uint32_t);

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf("\n%s: %f", args->name, buff);

		#else

			if( ImGui::InputScalar(taggedName, ImGuiDataType_U32, &buff) )
			{
				*_data = buff;
			}

		#endif
	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<uint32_t>* data )
	{
		ARGS_new* args = (ARGS_new*)data;
		bfu::SerializableVector<uint32_t>* _data = 
			(bfu::SerializableVector<uint32_t>*)args->dataPtr;

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf( "\n%s:", args->name );

		#else

			uint32_t size = _data->size();
			uint32_t step = 1;
			char id[32];
			snprintf(id, 32, "%s##%llu",  args->name, (size_t)data);


			if(ImGui::BeginTable(id, 1, ImGuiTableFlags_Borders))
			{
			    ImGui::TableSetupColumn("Vector size:", ImGuiTableColumnFlags_WidthStretch);

				ImGui::TableHeadersRow();
				
				if( ImGui::InputScalar(args->it->name, ImGuiDataType_U32, &size, &step, NULL, "%d") )
				{
					_data->resize( size );
				}

				for(int i=0; i<_data->size(); ++i)
				{
					uint32_t buff = (*_data)[i];
					char name[16];
					snprintf(name, 16, "%d", i);

			        ImGui::TableNextRow();
				    ImGui::TableSetColumnIndex( 0 );

					if( ImGui::InputScalar(name, ImGuiDataType_U32, &buff) )
					{
						(*_data)[i] = buff;
					}
				}
				
			    ImGui::EndTable();
			}

		#endif
	}

	void ImGUISerializer::Serialize( uint64_t* data )
	{
		GENERATE_SERIALIZE_BODY(uint64_t);

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf("\n%s: %f", args->name, buff);

		#else

			if( ImGui::InputScalar(taggedName, ImGuiDataType_U64, &buff) )
			{
				*_data = buff;
			}

		#endif
	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<uint64_t>* data )
	{
		ARGS_new* args = (ARGS_new*)data;
		bfu::SerializableVector<uint64_t>* _data = 
			(bfu::SerializableVector<uint64_t>*)args->dataPtr;

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf( "\n%s:", args->name );

		#else

			uint32_t size = _data->size();
			uint32_t step = 1;
			char id[32];
			snprintf(id, 32, "%s##%llu",  args->name, (size_t)data);


			if(ImGui::BeginTable(id, 1, ImGuiTableFlags_Borders))
			{
			    ImGui::TableSetupColumn("Vector size:", ImGuiTableColumnFlags_WidthStretch);

				ImGui::TableHeadersRow();
				
				if( ImGui::InputScalar(args->it->name, ImGuiDataType_U32, &size, &step, NULL, "%d") )
				{
					_data->resize( size );
				}

				for(int i=0; i<_data->size(); ++i)
				{
					uint64_t buff = (*_data)[i];
					char name[16];
					snprintf(name, 16, "%d", i);

			        ImGui::TableNextRow();
				    ImGui::TableSetColumnIndex( 0 );

					if( ImGui::InputScalar(name, ImGuiDataType_U64, &buff) )
					{
						(*_data)[i] = buff;
					}
				}
				
			    ImGui::EndTable();
			}

		#endif
	}


	void ImGUISerializer::Serialize( int8_t* data )
	{
		GENERATE_SERIALIZE_BODY(int8_t);

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf("\n%s: %f", args->name, buff);

		#else

			if( ImGui::InputScalar(taggedName, ImGuiDataType_S8, &buff) )
			{
				*_data = buff;
			}

		#endif
	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<int8_t>* data )
	{
		ARGS_new* args = (ARGS_new*)data;
		bfu::SerializableVector<int8_t>* _data = 
			(bfu::SerializableVector<int8_t>*)args->dataPtr;

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf( "\n%s:", args->name );

		#else

			uint32_t size = _data->size();
			uint32_t step = 1;
			char id[32];
			snprintf(id, 32, "%s##%llu",  args->name, (size_t)data);


			if(ImGui::BeginTable(id, 1, ImGuiTableFlags_Borders))
			{
			    ImGui::TableSetupColumn("Vector size:", ImGuiTableColumnFlags_WidthStretch);

				ImGui::TableHeadersRow();
				
				if( ImGui::InputScalar(args->it->name, ImGuiDataType_U32, &size, &step, NULL, "%d") )
				{
					_data->resize( size );
				}

				for(int i=0; i<_data->size(); ++i)
				{
					int8_t buff = (*_data)[i];
					char name[16];
					snprintf(name, 16, "%d", i);

			        ImGui::TableNextRow();
				    ImGui::TableSetColumnIndex( 0 );

					if( ImGui::InputScalar(name, ImGuiDataType_S8, &buff) )
					{
						(*_data)[i] = buff;
					}
				}
				
			    ImGui::EndTable();
			}

		#endif
	}

	void ImGUISerializer::Serialize( int16_t* data )
	{
		GENERATE_SERIALIZE_BODY(int16_t);

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf("\n%s: %f", args->name, buff);

		#else

			if( ImGui::InputScalar(taggedName, ImGuiDataType_S16, &buff) )
			{
				*_data = buff;
			}

		#endif
	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<int16_t>* data )
	{
		ARGS_new* args = (ARGS_new*)data;
		bfu::SerializableVector<int16_t>* _data = 
			(bfu::SerializableVector<int16_t>*)args->dataPtr;

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf( "\n%s:", args->name );

		#else

			uint32_t size = _data->size();
			uint32_t step = 1;
			char id[32];
			snprintf(id, 32, "%s##%llu",  args->name, (size_t)data);


			if(ImGui::BeginTable(id, 1, ImGuiTableFlags_Borders))
			{
			    ImGui::TableSetupColumn("Vector size:", ImGuiTableColumnFlags_WidthStretch);

				ImGui::TableHeadersRow();
				
				if( ImGui::InputScalar(args->it->name, ImGuiDataType_U32, &size, &step, NULL, "%d") )
				{
					_data->resize( size );
				}

				for(int i=0; i<_data->size(); ++i)
				{
					int16_t buff = (*_data)[i];
					char name[16];
					snprintf(name, 16, "%d", i);

			        ImGui::TableNextRow();
				    ImGui::TableSetColumnIndex( 0 );

					if( ImGui::InputScalar(name, ImGuiDataType_S16, &buff) )
					{
						(*_data)[i] = buff;
					}
				}
				
			    ImGui::EndTable();
			}

		#endif
	}

	void ImGUISerializer::Serialize( int32_t* data )
	{
		GENERATE_SERIALIZE_BODY(int32_t);

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf("\n%s: %f", args->name, buff);

		#else

			if( ImGui::InputScalar(taggedName, ImGuiDataType_S32, &buff) )
			{
				*_data = buff;
			}

		#endif
	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<int32_t>* data )
	{
		ARGS_new* args = (ARGS_new*)data;
		bfu::SerializableVector<int32_t>* _data = 
			(bfu::SerializableVector<int32_t>*)args->dataPtr;

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf( "\n%s:", args->name );

		#else

			uint32_t size = _data->size();
			uint32_t step = 1;
			char id[32];
			snprintf(id, 32, "%s##%llu",  args->name, (size_t)data);


			if(ImGui::BeginTable(id, 1, ImGuiTableFlags_Borders))
			{
			    ImGui::TableSetupColumn("Vector size:", ImGuiTableColumnFlags_WidthStretch);

				ImGui::TableHeadersRow();
				
				if( ImGui::InputScalar(args->it->name, ImGuiDataType_U32, &size, &step, NULL, "%d") )
				{
					_data->resize( size );
				}

				for(int i=0; i<_data->size(); ++i)
				{
					int32_t buff = (*_data)[i];
					char name[16];
					snprintf(name, 16, "%d", i);

			        ImGui::TableNextRow();
				    ImGui::TableSetColumnIndex( 0 );

					if( ImGui::InputScalar(name, ImGuiDataType_S32, &buff) )
					{
						(*_data)[i] = buff;
					}
				}
				
			    ImGui::EndTable();
			}

		#endif
	}

	void ImGUISerializer::Serialize( int64_t* data )
	{
		GENERATE_SERIALIZE_BODY(int64_t);

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf("\n%s: %f", args->name, buff);

		#else

			if( ImGui::InputScalar(taggedName, ImGuiDataType_S64, &buff) )
			{
				*_data = buff;
			}

		#endif
	}

	void ImGUISerializer::Serialize( bfu::SerializableVector<int64_t>* data )
	{
		ARGS_new* args = (ARGS_new*)data;
		bfu::SerializableVector<int64_t>* _data = 
			(bfu::SerializableVector<int64_t>*)args->dataPtr;

		#ifdef SERIALIZATIO_NOBJECT_TESTS

			printf( "\n%s:", args->name );

		#else

			uint32_t size = _data->size();
			uint32_t step = 1;
			char id[32];
			snprintf(id, 32, "%s##%llu",  args->name, (size_t)data);


			if(ImGui::BeginTable(id, 1, ImGuiTableFlags_Borders))
			{
			    ImGui::TableSetupColumn("Vector size:", ImGuiTableColumnFlags_WidthStretch);

				ImGui::TableHeadersRow();
				
				if( ImGui::InputScalar(args->it->name, ImGuiDataType_U32, &size, &step, NULL, "%d") )
				{
					_data->resize( size );
				}

				for(int i=0; i<_data->size(); ++i)
				{
					int64_t buff = (*_data)[i];
					char name[16];
					snprintf(name, 16, "%d", i);

			        ImGui::TableNextRow();
				    ImGui::TableSetColumnIndex( 0 );

					if( ImGui::InputScalar(name, ImGuiDataType_S64, &buff) )
					{
						(*_data)[i] = buff;
					}
				}
				
			    ImGui::EndTable();
			}

		#endif
	}










	void ImGUISerializer::Deserialize( bfu::SerializableClassInterface* data )
	{
		#ifndef SERIALIZATIO_NOBJECT_TESTS
            ImGui::EndTable();
		#endif
	}

	void ImGUISerializer::Deserialize( bfu::SerializableVector<bfu::SerializableClassInterface>* data )
	{
		#ifndef SERIALIZATIO_NOBJECT_TESTS
            ImGui::EndTable();
		#endif
	}

	void ImGUISerializer::Deserialize( float* data ){};
	void ImGUISerializer::Deserialize( bfu::SerializableVector<float>* data ){};
	void ImGUISerializer::Deserialize( bool* data ){};
	void ImGUISerializer::Deserialize( bfu::SerializableVector<bool>* data ){};
	void ImGUISerializer::Deserialize( bfu::stream* data ){};
	void ImGUISerializer::Deserialize( bfu::SerializableVector<bfu::stream>* data ){};
	void ImGUISerializer::Deserialize( bfu::string* data ){};
	void ImGUISerializer::Deserialize( bfu::SerializableVector<bfu::string>* data ){};

	void ImGUISerializer::Deserialize( uint8_t* data ){};
	void ImGUISerializer::Deserialize( bfu::SerializableVector<uint8_t>* data ){};
	void ImGUISerializer::Deserialize( uint16_t* data ){};
	void ImGUISerializer::Deserialize( bfu::SerializableVector<uint16_t>* data ){};
	void ImGUISerializer::Deserialize( uint32_t* data ){};
	void ImGUISerializer::Deserialize( bfu::SerializableVector<uint32_t>* data ){};
	void ImGUISerializer::Deserialize( uint64_t* data ){};
	void ImGUISerializer::Deserialize( bfu::SerializableVector<uint64_t>* data ){};

	void ImGUISerializer::Deserialize( int8_t* data ){};
	void ImGUISerializer::Deserialize( bfu::SerializableVector<int8_t>* data ){};
	void ImGUISerializer::Deserialize( int16_t* data ){};
	void ImGUISerializer::Deserialize( bfu::SerializableVector<int16_t>* data ){};
	void ImGUISerializer::Deserialize( int32_t* data ){};
	void ImGUISerializer::Deserialize( bfu::SerializableVector<int32_t>* data ){};
	void ImGUISerializer::Deserialize( int64_t* data ){};
	void ImGUISerializer::Deserialize( bfu::SerializableVector<int64_t>* data ){};
}