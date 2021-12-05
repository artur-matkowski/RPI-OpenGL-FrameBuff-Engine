#ifndef H_TESTSResourceTracker
#define H_TESTSResourceTracker
#include "log.hpp"
#include "object.hpp"

#define TESTS
#include "ResourceTrackerManager.hpp"

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

		asapi::ResourceTrackerManager res;
		std::vector< resourceEntry > currentResources;

		static void Command(const char *format, ...);

	public:
		TestsResourceTracker(const char* testProjectPath);
		~TestsResourceTracker();

		void CreateResource(const char* filename, const char* content);
		void MoveResource(const char* source, const char* destination);
		void RemoveResource(const char* filename);
		void AppendResource(const char* filename, const char* content);

		bool TestDataCohesion();

	};


#endif