#include "object.hpp"
#include "Systems.hpp"


bfu::MemBlockBase* allocator = nullptr;
bfu::StdAllocatorMemBlock a;

void * operator new(std::size_t size)
{
	if(allocator!=0)
		return allocator->allocate(1, size, 1);
	else
	{
		return a.allocate(1, size, 1);
	}
} 
  
void operator delete(void * p) noexcept
{
	DEALLOCATE_GLOBAL(p);
}


void SetNewAllocator(bfu::MemBlockBase* all)
{
	allocator = all;
}