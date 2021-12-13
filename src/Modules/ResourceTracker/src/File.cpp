#include "File.hpp"
#include "object.hpp"

#include <fcntl.h>
#include <sys/mman.h>
#include <dirent.h> 
#include <sys/stat.h>

#include "bfu.hpp"



namespace asapi
{
	std::string GetPath(const char* fullFilePath)
	{
		std::string ret(fullFilePath);

		ret.erase(ret.find_last_of('/'), std::string::npos);

		return ret;
	}

	bool mkdir_recursivly(const std::string& arg_path)
	{
		if( arg_path.size() <= 1 )
		{
			return false;
		}


		if( mkdir( arg_path.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH ) == 0 )
		{
			return true;
		}
		else
		{
			std::string path = GetPath( arg_path.c_str() );

			if( mkdir_recursivly( path ) ){
				return ( 0 == mkdir( arg_path.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH ) );
			}
			else{
				return false;
			}
		}
	}


	void ListFiles(std::vector< std::string >& out
				, const std::vector< std::string >& extensions
				, ListingStrategy strategy
				, const char* dirname
				, const char* subdirname)
	{
		int i;
		DIR* d_fh;
		struct dirent* entry;
		char path[MAX_PATH_SIZE];

		strncpy(path, dirname, MAX_PATH_SIZE-1);
		if( subdirname!=0 )
		{
			strncat(path, "/", MAX_PATH_SIZE-1);
			strncat(path, subdirname, MAX_PATH_SIZE-1);
		}


		if( (d_fh = opendir(path)) == NULL) 
		{
			log::error << "Couldn't open directory, errno: " << errno << "\n\tDirname: " << path << " " << std::endl;
			return;
		}

		// if( (entry=readdir(d_fh)) == NULL )
		// {
		// 	log::error << "readdir error: " << errno << std::endl;
		// 	return;
		// }
		
		
		while( (entry=readdir(d_fh)) != NULL )
		{

			/* Don't descend up the tree or include the current directory */
			if( (strncmp(entry->d_name, "..", 2) != 0) &&
				(strncmp(entry->d_name, ".", 1) != 0) )
			{
				bool skipfile = false;
				if( strategy==ListingStrategy::blacklist )
				{
					for(auto it = extensions.begin(); it!=extensions.end(); ++it)
					{
						if(strstr(entry->d_name, it->c_str()) != 0 )
						{
							skipfile = true;
							break;
						}
					}
				}
				else // if( strategy==ListingStrategy::whitelist )
				{
					skipfile = true;
					for(auto it = extensions.begin(); it!=extensions.end(); ++it)
					{
						if(strstr(entry->d_name, it->c_str()) != 0 )
						{
							skipfile = false;
							break;
						}
					}
				}

				if( !skipfile )
				{
					bzero(path, MAX_PATH_SIZE);
					if( subdirname!=0 )
					{
						strncpy(path, subdirname, MAX_PATH_SIZE-1);
						strncat(path, "/", MAX_PATH_SIZE-1);
					}
					strncat(path, entry->d_name, MAX_PATH_SIZE-1);

					/* If it's a directory print it's name and recurse into it */
					if (entry->d_type == DT_DIR) 
					{
						ListFiles(out, extensions, strategy, dirname, path);
					}
					else //if not dir
					{
						out.emplace_back(path);
					}
				}

			}
		}

		closedir(d_fh);
	}


	FILE::MMAP::MMAP(FILE::MMAP&& cp) noexcept
	{
		Close();
		
		data = cp.data;
		fd = cp.fd;
		sb = cp.sb;

		cp.data = nullptr;
		cp.fd = -1;
	}

	FILE::MMAP& FILE::MMAP::operator=(FILE::MMAP&& cp)
	{
		Close();

		data = cp.data;
		fd = cp.fd;
		sb = cp.sb;

		cp.data = nullptr;
		cp.fd = -1;

		return *this;
	}


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
			log::warning << "Can not open file: " << filename << " attempting to rebuild directory path." << std::endl;
			std::string path = GetPath(filename);
			if( mkdir_recursivly( path.c_str() ) )
			{
				fd = open(filename, O_RDWR | O_CREAT | O_TRUNC | O_SYNC, (mode_t)0666);
				if (fd == -1)
				{
					log::error << "Can not open file: " << filename << std::endl;
					return;
				}
			}
			else
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
			std::string path = GetPath(filename);
			if( mkdir_recursivly( path.c_str() ) )
			{
				fd = open(filename, O_RDWR | O_CREAT | O_TRUNC | O_SYNC, (mode_t)0666);
				if (fd == -1)
				{
					log::error << "Can not open file: " << filename << std::endl;
					return;
				}
			}
			else
				return;
			return;
		}
	}


	void FILE::STREAM::Write(const char* buff, const int size)
	{
		write(fd, buff, size);
	}


	void FILE::Remove(const char* filename)
	{
		unlink(filename);
	}
}