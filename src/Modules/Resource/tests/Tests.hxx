#ifndef _H_TESTS
#define _H_TESTS
#include "ResourceManager.hpp"
#include "bfu.hpp"

class TestResource: public asapi::ResourceBase
{
protected:

	virtual void LoadResource()
	{

	}
	virtual void DispouseResource()
	{

	}

public:

	TestResource(uint64_t resourceIDlink)
		:asapi::ResourceBase(resourceIDlink)
	{

	}
	TestResource(const TestResource& cp)
		:asapi::ResourceBase(cp)
	{

	}
	virtual ~TestResource()
	{
		
	}
};

class Tests
{
	asapi::ResourceManager manager;
public:
	Tests();
	~Tests();
};


#endif