#ifndef _H_TESTS
#define _H_TESTS
#include "ResourceManager.hpp"
#include "bfu.hpp"

class TestResource: public asapi::ResourceBase
{
protected:

	virtual void LoadResource() override
	{

	}
	virtual void DispouseResource() override
	{

	}
	
	TestResource(uint64_t resourceIDlink)
		:asapi::ResourceBase(resourceIDlink)
	{

	}

public:

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