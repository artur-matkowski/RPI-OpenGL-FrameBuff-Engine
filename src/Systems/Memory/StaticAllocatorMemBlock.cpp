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
		size_t freeMemory = getFreeMemory()-sizeof(bfu::MemChunkHeader);

		void* tmp = m_buffFreePtr + sizeof(bfu::MemChunkHeader);

		if ( tmp = std::align(alignOf, sizeOf, tmp, freeMemory ))
        {
            void* result = m_buffFreePtr = tmp;
            size_t size = sizeOf * elements;
            size = size > 0 ? size : 1;
            m_buffFreePtr = (void*)((size_t) m_buffFreePtr + size);

	        bfu::MemChunkHeader* headerInfo = (bfu::MemChunkHeader*)((size_t)result-(size_t)sizeof(bfu::MemChunkHeader));
	        headerInfo->m_MemBlockOwner = this;
	        headerInfo->m_sizeOfChunk = size + sizeof(bfu::MemChunkHeader);

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
	    bfu::MemChunkHeader* headerInfo = bfu::MemChunkHeader::InitFromLifePtr(p);
		m_deallocatedMemory += headerInfo->m_sizeOfChunk;

		memset(p, 0, headerInfo->m_sizeOfChunk-sizeof( bfu::MemChunkHeader) );

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