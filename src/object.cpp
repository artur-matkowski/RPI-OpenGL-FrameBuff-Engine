#include "object.hpp"
#include "Systems.hpp"

static bool isInitTime = true;

void * operator new(std::size_t size)
{
	if(isInitTime)
		return asapi::SYSTEMS::ALLOCATE<char>(size);
	else
		return asapi::SYSTEMS::STD_ALLOCATOR->allocate(1, size, 1);
} 
  
void operator delete(void * p) noexcept
{ 
	auto systemsAlloc = *asapi::SYSTEMS::SYSTEMS_ALLOCATOR;

	if( systemsAlloc.owns(p) )
    	return asapi::SYSTEMS::DEALLOCATE((char*)p, 0);
    else 
		return asapi::SYSTEMS::STD_ALLOCATOR->deallocate(p, 0);
}