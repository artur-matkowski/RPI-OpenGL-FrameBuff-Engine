#ifndef _H_TESTresBin
#define _H_TESTresBin
#include "ResourceTracker.hpp"

bool ProcessResourceTracker(asapi::ResourceTracker* in_currentResource, const char* in_projectPath, std::vector<asapi::SubResourceData>& out_resourceBinaries);


class TestResourceBinaries
{
public:
	TestResourceBinaries();
	~TestResourceBinaries();
	
};


#endif