#ifndef H_FILE
#define H_FILE
#include <sys/stat.h>
#include <unistd.h>
#include <cstdint>
#include <vector>
#include <string>


namespace asapi
{
	enum class ListingStrategy: char
	{ whitelist, blacklist };

	void ListFiles(std::vector< std::string >& out
					, const std::vector< std::string >& extensions
					, ListingStrategy strategy
					, const char* dirname
					, const char* subdirname = 0);


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


			MMAP(MMAP&& cp) noexcept;

			MMAP& operator=(MMAP&& cp);

			void InitForRead(const char* filename);
			bool TryInitForRead(const char* filename);
			void InitForWrite(const char* filename, size_t size);

			void Close();

			bool IsValid(){ return fd!=-1 && data!=nullptr; }
			void* Data(){ return data; }
			uint32_t Size(){ return sb.st_size; }
		};

		class STREAM
		{
			int fd = -1;

		public:
			STREAM(){};
			~STREAM();

			void Close();

			bool IsValid(){ return fd!=-1; }
			void InitForWrite(const char* filename);
			void Write(const char* buff, const int size);
		};

		static void Remove(const char*);
		static bool Move(const char* dest, const char* src);
		static bool GetFileNameWithExtension(char* out_filename, const int filename_size, const char* buff, const int buff_size);
		static bool GetFileNameWithoutExtension(char* out_filename, const int filename_size, const char* buff, const int buff_size);
		static bool FileExist(const char* path);
		static bool Touch(const char* path);
		static uint64_t GetModificationTime(const char* path);
	};
}

#endif