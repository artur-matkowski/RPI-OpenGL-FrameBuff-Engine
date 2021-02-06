#ifndef _H_MemoryManagmentSystem
#define _H_MemoryManagmentSystem
#include <bitforge/utils/bfu.hpp>
#include <vector>

namespace asapgl
{
	class GameObject;
	
	class MemoryManagmentSystem
	{
		//bfu::MonotonicMemBlock<1024*1024*10> 	SystemsMemoryBlock;
		bfu::MmappedMemBlock				 	SystemsMemoryBlock;
		bfu::MallocAllocator					m_operatorNEWstatistics;
		bfu::StdAllocatorMemBlock				m_StdAllocatorMemBlock;

		std::vector<bfu::MemBlockBase*, bfu::custom_allocator<bfu::MemBlockBase*>> v_memBlocks;

		bfu::MmappedMemBlock* 					p_memBlockCache = 0;
		void* 									p_memBlocksEnd = 0;

	public:

		MemoryManagmentSystem()
			:SystemsMemoryBlock((void*)1, 1024*1024*10, "SystemsMemoryBlock")
			,v_memBlocks(bfu::custom_allocator<bfu::MemBlockBase*>(&SystemsMemoryBlock))
		{
			v_memBlocks.reserve(16);
			v_memBlocks.push_back(&SystemsMemoryBlock);
			v_memBlocks.push_back(&m_operatorNEWstatistics);
			v_memBlocks.push_back(&m_StdAllocatorMemBlock);

			p_memBlocksEnd = SystemsMemoryBlock.end();

			p_memBlockCache = (bfu::MmappedMemBlock*) SystemsMemoryBlock.allocate( 1
																					, sizeof(bfu::MmappedMemBlock)
																					, alignof(bfu::MmappedMemBlock));
			/*
			new (p_memBlockCache) bfu::MmappedMemBlock((void*)(((size_t)p_memBlocksEnd)+1), 1024*1024*10, "TestMemoryBlock");

			p_memBlocksEnd = p_memBlockCache->end();
			v_memBlocks.push_back(p_memBlockCache);*/
		}


		#ifdef IS_EDITOR
		void OnGUI();
		#endif

		template<class T>
		T* allocateSystemInBlock (std::size_t n) 
		{
	  		T* ret = (T*)(SystemsMemoryBlock.allocate( n, sizeof(T), alignof(T) ));		
			return ret; 
		}

		template<class T>
		void deallocateSystemInBlock (T* p, std::size_t n) 
		{
	  		SystemsMemoryBlock.deallocate(p, n * sizeof(T));	
		}

		inline bfu::MmappedMemBlock& GetSystemsAllocator()
		{
			return SystemsMemoryBlock;
		}

		inline bfu::StdAllocatorMemBlock& GetStdAllocator()
		{
			return m_StdAllocatorMemBlock;
		}

		bfu::MmappedMemBlock* ObtainPrefabMemBlock(size_t size, GameObject* &ret_entryPointRaw, const char* description);
	};


}

#define ALLOCATE GetObject().MEMORY.allocateSystemInBlock
#define DEALLOCATE GetObject().MEMORY.deallocateSystemInBlock

#endif