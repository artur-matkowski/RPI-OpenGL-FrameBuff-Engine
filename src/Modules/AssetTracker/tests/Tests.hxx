#ifndef H_TESTS
#define H_TESTS
#include "log.hpp"
#include "object.hpp"

#define TESTS
#include "AssetSystem.hpp"


class Tests
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

	asapi::AssetSystem res;
	std::vector< resourceEntry > currentResources;

	static void Command(const char *format, ...);

public:
	Tests(const char* testProjectPath);
	~Tests();

	void CreateResource(const char* filename, const char* content);
	void MoveResource(const char* source, const char* destination);
	void RemoveResource(const char* filename);

	bool TestDataCohesion();

	};


#endif