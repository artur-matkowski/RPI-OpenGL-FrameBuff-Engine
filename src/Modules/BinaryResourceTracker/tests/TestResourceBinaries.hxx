#ifndef _H_TESTresBin
#define _H_TESTresBin
#include <vector>
#define TESTS
#include "ResourceSystem.hpp"


class ResourceTXTProcessor
{
	static std::vector<std::string> data;
public:
	static void* LoadResource(const char* path)
	{
		asapi::FILE::MMAP file;

		file.InitForRead( path );

		if( !file.IsValid() )
		{
			log::error << "can not open resource binary: " << path << std::endl;
			return 0;
		}

		data.emplace_back( file.data() );

		return data.last().c_str();
	}

	static void UnloadResource(void* handle)
	{
		data.remove( handle );
	}

	static bool ProcessResource2Binary(const asapi::ResourceTracker& in_currentResource
											, asapi::FILE::MMAP* in_resourceFile
											, const char* in_projectPath
											, std::vector<asapi::SubResourceData>* out_resourceBinaries);

	static void GetSuportedFilesExtensions( std::vector<std::string>* out_suportedFileExtensions )
	{
		out_suportedFileExtensions->emplace_back(".txt");
	}
};

class ResourceTXTSharedReference: public ResourceSharedReferenceBase<ResourceTXTSharedReference, ResourceTXTProcessor>
{};



class TestResourceBinaries
{
	asapi::ResourceSystem<
		ResourceReference<ResourceTXTProcessor>
		> 										m_resourceSystem;


		ResourceTXTSharedReference 				m_testResource;
		
		static void Command(const char *format, ...);
public:
	TestResourceBinaries();
	~TestResourceBinaries();


	void CreateResource(const char* filename, const char* content);
	void MoveResource(const char* source, const char* destination);
	void RemoveResource(const char* filename);
	void AppendResource(const char* filename, const char* content);
	
};


#endif