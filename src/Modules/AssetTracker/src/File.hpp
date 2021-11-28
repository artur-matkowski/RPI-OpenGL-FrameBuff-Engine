#ifndef H_FILE
#define H_FILE
#include <sys/stat.h>
#include <unistd.h>
#include <cstdint>
#include <string>


namespace asapi
{
	struct FILE
	{
		class MMAP
		{
			void* data = nullptr;
			int fd = -1;
			struct stat sb;

		public:
			MMAP(){};
			~MMAP();

			void InitForRead(const char* filename);
			void InitForWrite(const char* filename, size_t size);

			void Close();

			bool IsValid(){ return fd!=-1; }
			void* Data(){ return data; }
			uint32_t Size(){ return sb.st_size; }

			std::string GetHash(const char* path);

		};

		class STREAM
		{
			int fd = -1;

		public:
			STREAM(){};
			~STREAM();

			void Close();

			void InitForWrite(const char* filename);
			void Write(const char* buff, const int size);
		};
	};
}

#endif