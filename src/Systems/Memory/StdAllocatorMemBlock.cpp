#include "StdAllocatorMemBlock.hpp"


namespace asapi
{
	StdAllocatorMemBlock::StdAllocatorMemBlock(const char* name)
		:MemBlockBase( name )
		,p_allocatedMemory(new size_t(0))
		,p_deallocatedMemory(new size_t(0))
		,p_allocationCount(new int(0))
		,p_deallocationCount(new int(0))
	{
	}

	StdAllocatorMemBlock::StdAllocatorMemBlock(const StdAllocatorMemBlock& cp)
		:MemBlockBase( cp.GetDescription() )
		,p_allocatedMemory(cp.p_allocatedMemory)
		,p_deallocatedMemory(cp.p_deallocatedMemory)
		,p_allocationCount(cp.p_allocationCount)
		,p_deallocationCount(cp.p_deallocationCount)
	{
	}
}