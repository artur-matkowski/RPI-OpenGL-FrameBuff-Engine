#ifndef H_AsapiStdAllocatorMemBlock
#define H_AsapiStdAllocatorMemBlock
#include <bitforge/utils/bfu.hpp>
#include "SharedPtr.hpp"

namespace asapi
{
	class StdAllocatorMemBlock: public bfu::MemBlockBase
	{
		SharedPtr<size_t> 	p_allocatedMemory;
		SharedPtr<size_t> 	p_deallocatedMemory;
		SharedPtr<int> 		p_allocationCount;
		SharedPtr<int> 		p_deallocationCount;

	public:
		StdAllocatorMemBlock(const char* name = "StdAllocatorMemBlock");
		StdAllocatorMemBlock(const StdAllocatorMemBlock& cp);

		virtual void* allocate (int elements, std::size_t sizeOf, std::size_t alignOf)
	    {
	    	*p_allocatedMemory += sizeOf * elements;

	        void* ret = aligned_alloc(alignOf, sizeOf * elements);

	        ++*p_allocationCount;

	        return ret;
	    }

		virtual void deallocate (void* p, std::size_t n)
	    {
	    	*p_deallocatedMemory += n;
	    	++*p_deallocationCount;

	        free(p);
	    };
		virtual size_t getFreeMemory()
	    {
		    return 0;
	    };
		virtual size_t getUsedMemory() 
		{
			return *p_allocatedMemory;
		}
		virtual void* getRefPtr()
		{
			return 0;
		}
		virtual void* getMemPtr()
		{
			return nullptr;
		}
		virtual bool owns(void* ptr)
		{
			return false;
		}

		virtual int GetAllocationsCount() {return *p_allocationCount;}
		virtual int GetDeallocationsCount() {return *p_deallocationCount;}


	};
}

#endif