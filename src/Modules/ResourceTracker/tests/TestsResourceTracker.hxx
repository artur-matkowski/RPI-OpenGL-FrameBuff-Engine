#ifndef H_TESTSResourceTracker
#define H_TESTSResourceTracker
#include "log.hpp"
#include "object.hpp"

#define TESTS
#include "ResourceTrackerManager.hpp"

	bool ProcessResourceTracker(asapi::ResourceTracker* in_currentResource, const char* in_projectPath, std::vector<asapi::SubResourceData>& out_resourceBinaries);


	class ResourceTXTProcessor
	{
	public:
		static void ProcessResource2Binary(asapi::ResourceTracker* in_currentResource
											, const char* in_projectPath
											, std::vector<asapi::SubResourceData>& out_resourceBinaries);

		static void GetSuportedFilesExtensions( std::vector<std::string>& out_suportedFileExtensions )
		{
			out_suportedFileExtensions.emplace_back(".txt");
		}
	};

	class TestsResourceTracker
	{
		struct resourceEntry
		{
			std::string filename;
			std::string content;
			std::string content_hash;
			uint64_t 	resourceLink = 0;
		};

		int introducedResources = 0;
		int movedResources = 0;
		int removedResources = 0;


		char m_testProjectPath[MAX_PATH_SIZE];
		char m_ResourceFilesDirPath[MAX_PATH_SIZE];

		asapi::ResourceTrackerManager resourceTrackerManager;
		std::vector< resourceEntry > currentResources;

		static void Command(const char *format, ...);

	public:
		TestsResourceTracker(const char* testProjectPath, bool (*callback)(asapi::ResourceTracker* in_currentResource, const char* in_projectPath, std::vector<asapi::SubResourceData>& out_resourceBinaries) = ProcessResourceTracker);
		~TestsResourceTracker();

		void CreateResource(const char* filename, const char* content);
		void MoveResource(const char* source, const char* destination);
		void RemoveResource(const char* filename);
		void AppendResource(const char* filename, const char* content);

		bool TestDataCohesion();

		void Print();

	};


#endif