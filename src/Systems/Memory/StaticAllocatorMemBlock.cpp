#include "StaticAllocatorMemBlock.hpp"

namespace asapi
{
	StaticAllocatorMemBlock::StaticAllocatorMemBlock(void* memStart, size_t memSize, const char* name)
		:bfu::MemBlockBase(name)
	{
		m_buffFreePtr = m_buffStartPtr = memStart;
		std::memset(m_buffFreePtr, 0, memSize);
		m_buffEndPtr = (void*)((size_t)m_buffStartPtr + memSize);
	}
	StaticAllocatorMemBlock::StaticAllocatorMemBlock(const StaticAllocatorMemBlock& cp)
		:bfu::MemBlockBase(cp.m_memBlockDescriptor)
	{
		m_buffStartPtr = cp.m_buffStartPtr;
		m_buffFreePtr = cp.m_buffFreePtr;
		m_buffEndPtr = cp.m_buffEndPtr;
		m_deallocatedMemory = cp.m_deallocatedMemory;
	}
	StaticAllocatorMemBlock::~StaticAllocatorMemBlock()
	{
		//no deallocation on pre suplied buffer
	}


	void* StaticAllocatorMemBlock::allocate (int elements, std::size_t sizeOf, std::size_t alignOf)
	{
		size_t size = getFreeMemory();

		void* tmp = m_buffFreePtr;

		if ( m_buffFreePtr = std::align(alignOf, sizeOf, m_buffFreePtr, size ))
        {
            void* result = m_buffFreePtr;
            size_t size = sizeOf * elements;
            size = size > 0 ? size : 1;
            m_buffFreePtr = (void*)((size_t) m_buffFreePtr + size);


            if(m_buffFreePtr >= m_buffEndPtr)
	        {
	            //std::cout << "Failed to allocate memory by MonotonicMemBlock, requested size: " << sizeOf * elements << std::endl;
					//std::cout.flush();
					return nullptr;
	        }

			++m_allocationCount;
			#ifdef DEBUG_MEMORY_ALLOC
			logAlloc(	result, 
	    			size, 
	    			m_memBlockDescriptor,
	    			getUsedMemory(),
	    			getFreeMemory(),
	    			m_deallocatedMemory,
	    			m_allocationCount,
	    			m_deallocationCount,
	    			m_buffStartPtr);
			#endif

            return result;
        }
        return nullptr;
	}

	void StaticAllocatorMemBlock::deallocate (void* p, std::size_t n) 
	{
		m_deallocatedMemory += n;
		memset(p, 0, n);
		if( (size_t)p+n==(size_t)m_buffFreePtr)
		{
            //std::cout << "Regaining memory becouse deallocate was called right after allocate on the same ptr" << std::endl;
			//std::cout.flush();
			m_buffFreePtr = (void*)((size_t)m_buffFreePtr - n);
		}

		++m_deallocationCount;
		#ifdef DEBUG_MEMORY_ALLOC
			logDealloc(	p, 
			n, 
			m_memBlockDescriptor,
			getUsedMemory(),
			getFreeMemory(),
			m_deallocatedMemory,
			m_allocationCount,
			m_deallocationCount,
			m_buffStartPtr);
		#endif
	}

	void StaticAllocatorMemBlock::free()
	{
		m_buffFreePtr = m_buffStartPtr;
	}


	size_t StaticAllocatorMemBlock::getFreeMemory()
	{
		return (size_t)m_buffEndPtr- (size_t)m_buffFreePtr;
	}
	size_t StaticAllocatorMemBlock::getUsedMemory()
	{
		return (size_t)m_buffFreePtr- (size_t)m_buffStartPtr;
	}
	void* StaticAllocatorMemBlock::getRefPtr()
	{
		return this;
	}
	void* StaticAllocatorMemBlock::getMemPtr()
	{
		return m_buffStartPtr;
	}
	bool StaticAllocatorMemBlock::owns(void* ptr)
	{
		if((size_t)m_buffStartPtr<(size_t)ptr && (size_t)ptr < (size_t)m_buffEndPtr)
			return true;
		
		return false;
	}
}