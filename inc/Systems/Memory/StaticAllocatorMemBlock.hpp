#ifndef H_StaticAllocatorMemBlock
#define H_StaticAllocatorMemBlock
#include <bitforge/utils/bfu.hpp>

namespace asapi
{
	class StaticAllocatorMemBlock: public bfu::MemBlockBase
	{
		void* m_buffStartPtr = 0;
		void* m_buffFreePtr = 0;
		void* m_buffEndPtr = 0;
		size_t m_deallocatedMemory = 0;
	public:
		StaticAllocatorMemBlock(void* memStart, size_t memSize, const char* name = "StaticAllocatorMemBlock");
		StaticAllocatorMemBlock(const StaticAllocatorMemBlock& cp);
		~StaticAllocatorMemBlock();

		virtual void* allocate (int elements, std::size_t sizeOf, std::size_t alignOf);

		virtual void deallocate (void* p, std::size_t n);

		void free();


		size_t getFreeMemory();
		size_t getUsedMemory();
		void* getRefPtr();
		void* getMemPtr();
		virtual bool owns(void* ptr);
		
	};
}

#endif