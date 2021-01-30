#include "object.hpp"
#include "Systems.hpp"

static bool isInitTime = true;

void * operator new(std::size_t size)
{
	if(isInitTime)
		return asapgl::SYSTEMS::ALLOCATE<char>(size);
	else
		return asapgl::SYSTEMS::GetObject().MEMORY.GetStdAllocator().allocate(1, size, 1);
} 
  
void operator delete(void * p) noexcept
{ 
	auto systemsAlloc = asapgl::SYSTEMS::GetObject().MEMORY.GetSystemsAllocator();

	if( systemsAlloc.owns(p) )
    	return asapgl::SYSTEMS::DEALLOCATE((char*)p, 0);
    else 
		return asapgl::SYSTEMS::GetObject().MEMORY.GetStdAllocator().deallocate(p, 0);
}