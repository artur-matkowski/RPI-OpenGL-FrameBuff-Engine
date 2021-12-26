#ifndef _H_TESTresBin
#define _H_TESTresBin
#include <vector>
#define TESTS
#include "object.hpp"
#include "ResourceSystem2.hpp"


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

		data.emplace_back( (const char*)file.Data() );

		return (void*)data.back().c_str();
	}

	static void UnloadResource(void* handle)
	{
		for(auto it = data.begin(); it!=data.end(); it++)
		{
			if( it->c_str() == handle )
			{
				data.erase( it );
				break;
			}
		}
	}

	static bool ProcessResource2Binary(const asapi::ResourceTracker& in_currentResource
											, asapi::FILE::MMAP* in_resourceFile
											, const char* in_projectPath
											, std::vector<asapi::SubResourceData>* out_resourceBinaries);

	static const char* GetSuportedResourceFileExtension()
	{
		return ".txt";
	}

	static const char* GetBinaryOutputFileExtension()
	{
		return ".txt";
	}

	friend bfu::stream& operator<<(bfu::stream&, const ResourceTXTProcessor& );	
};


class ResourceTXTSharedReference: public asapi::ResourceSharedReferenceBase<ResourceTXTSharedReference, ResourceTXTProcessor>
{
public:
	ResourceTXTSharedReference(){}

	ResourceTXTSharedReference( ResourceTXTSharedReference&& cp )
	{
		m_resourcePtr = cp.m_resourcePtr;
		cp.m_resourcePtr = nullptr;
	}
};



class TestResourceBinaries
{
	struct resourceEntry
		{
			std::string filename;
			std::string content;
			std::string content_hash;
			uint64_t 	resourceLink = 0;
		};

		char m_testProjectPath[MAX_PATH_SIZE];
		char m_ResourceFilesDirPath[MAX_PATH_SIZE];
		std::vector< resourceEntry > currentResources;
		
		int introducedResources = 0;
		int movedResources = 0;
		int removedResources = 0;




	asapi::ResourceSystem<
			ResourceTXTProcessor
			> 										m_resourceSystem;


		ResourceTXTSharedReference 				m_testResource;
		
		static void Command(const char *format, ...);
public:
	TestResourceBinaries(const char* testProjectPath);
	~TestResourceBinaries(){};


	void CreateResource(const char* filename, const char* content);
	void MoveResource(const char* source, const char* destination);
	void RemoveResource(const char* filename);
	void AppendResource(const char* filename, const char* content);

	bool TestDataCohesion();

	friend bfu::stream& operator<<(bfu::stream&, const TestResourceBinaries& );
	
};


#endif