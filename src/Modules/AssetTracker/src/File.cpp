#include "File.hpp"

#include <fcntl.h>
#include <sys/mman.h>

#include "sha256.h"
#include "bfu.hpp"



namespace asapi
{
	void FILE::MMAP::InitForRead(const char* filename)
	{
		fd = open(filename, O_RDONLY );
		if (fd == -1)
		{
			log::error << "Can not open file: " << filename << std::endl;
			return;
		}

		if (fstat(fd, &sb) == -1) 
		{
			log::error << "Can not fstat file: " << filename << std::endl;
			return;
		}

		if( sb.st_size==0 )
		{
			log::error << "File size is 0: " << filename << std::endl;
			return;
		}

		data = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

		if (data == MAP_FAILED)
			log::error << "Failed to mmap file: " << filename << std::endl;		
	}

	void FILE::MMAP::InitForWrite(const char* filename, size_t size)
	{
		unlink(filename);
		fd = open(filename, O_RDWR | O_CREAT | O_TRUNC | O_SYNC, (mode_t)0666);
		if (fd == -1)
		{
			log::error << "Can not open file: " << filename << std::endl;
			return;
		}

		const int pageSize = getpagesize();
		sb.st_size = size / pageSize + pageSize;

		data = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		ftruncate(fd, sb.st_size);
		bzero(data, sb.st_size);

		if (data == MAP_FAILED)
			log::error << "Failed to mmap file: " << filename << std::endl;	
	}

	FILE::MMAP::~MMAP()
	{
		if(data != MAP_FAILED && data != nullptr)
		{
			msync(data, sb.st_size, MS_SYNC);
			munmap(data, sb.st_size);
		}
		if(fd!=-1)
			close(fd);
	}

	void FILE::MMAP::Close()
	{
		if(data != MAP_FAILED && data != nullptr)
		{
			msync(data, sb.st_size, MS_SYNC);
			munmap(data, sb.st_size);
		}
		if(fd!=-1)
			close(fd);

		data = nullptr;
		fd = -1;
	}

	std::string FILE::MMAP::GetHash(const char* path)
	{
		struct stat attrib;

		if( stat(path, &attrib)!=0 )
    	{
    		log::error << "Could not find file " << path << std::endl;
    	}

		SHA256 sha256; 
		sha256(Data(), Size());

		return sha256.getHash();
	}


	FILE::STREAM::~STREAM()
	{
		Close();
	}

	void FILE::STREAM::Close()
	{
		if(fd!=-1)
			close(fd);
		
		fd = -1;
	}


	void FILE::STREAM::InitForWrite(const char* filename)
	{
		unlink(filename);
		fd = open(filename, O_RDWR | O_CREAT | O_TRUNC | O_SYNC, (mode_t)0666);
		if (fd == -1)
		{
			log::error << "Can not open file: " << filename << std::endl;
			return;
		}
	}


	void FILE::STREAM::Write(const char* buff, const int size)
	{
		write(fd, buff, size);
	}
}