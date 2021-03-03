#include "object.hpp"
#include "Systems.hpp"
#include "EarlyAlocMemBlock.hpp"

asapi::EarlyAlocMemBlock<1024*1024>* early_allocator;

bfu::MemBlockBase* allocator = nullptr;

void * operator new(std::size_t size)
{
	if(allocator==nullptr)
	{
		allocator = early_allocator = (asapi::EarlyAlocMemBlock<1024*1024>*)malloc(sizeof(asapi::EarlyAlocMemBlock<1024*1024>));
		new (early_allocator) asapi::EarlyAlocMemBlock<1024*1024>("Early [operator new()] allocator (stack)");
	}

	return allocator->allocate(1, size, 1);
} 
  
void operator delete(void * p) noexcept
{ 
	auto systemsAlloc = *asapi::SYSTEMS::SYSTEMS_ALLOCATOR;

	if( allocator!=0 && allocator->owns(p) )
		return allocator->deallocate(p, 0);
    else 
		return early_allocator->deallocate(p, 0);
}


bfu::MemBlockBase* SetNewAllocator(bfu::MemBlockBase* all)
{
	allocator = all;
	return early_allocator;
}