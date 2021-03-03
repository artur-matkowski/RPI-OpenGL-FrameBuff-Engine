#ifndef _H_MemoryManagmentSystem
#define _H_MemoryManagmentSystem
#include <bitforge/utils/bfu.hpp>
#include <vector>
#include "MmappedMemBlock.hpp"

namespace asapi
{
	class GameObject;
	
	class MemoryManagmentSystem
	{
		std::vector<bfu::MemBlockBase*, bfu::custom_allocator<bfu::MemBlockBase*>> v_memBlocks;

		MmappedMemBlock				 			SystemsMemoryBlock;
		bfu::MallocAllocator					m_operatorNEWstatistics;
		bfu::StdAllocatorMemBlock				m_StdAllocatorMemBlock;


		MmappedMemBlock* 						p_memBlockCache = 0;
		void* 									p_memBlocksEnd = 0;

	public:

		MemoryManagmentSystem()
			:v_memBlocks(bfu::custom_allocator<bfu::MemBlockBase*>(&SystemsMemoryBlock))
			,SystemsMemoryBlock("SystemsMemoryBlock", 1024*1024*10)
		{
			v_memBlocks.reserve(16);
			// v_memBlocks.push_back(&SystemsMemoryBlock); // self registerable
			v_memBlocks.push_back(&m_operatorNEWstatistics);
			v_memBlocks.push_back(&m_StdAllocatorMemBlock);

			p_memBlocksEnd = SystemsMemoryBlock.end();

			p_memBlockCache = (MmappedMemBlock*) SystemsMemoryBlock.allocate( 1
																			, sizeof(MmappedMemBlock)
																			, alignof(MmappedMemBlock));

		}


		#ifdef IS_EDITOR
		void OnGUI();
		#endif

		void RegisterMemBlock(bfu::MemBlockBase* memBlock);
    	void UnRegisterMemBlock(bfu::MemBlockBase* memBlock);

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

		inline MmappedMemBlock* GetSystemsAllocator()
		{
			return &SystemsMemoryBlock;
		}

		inline bfu::StdAllocatorMemBlock* GetStdAllocator()
		{
			return &m_StdAllocatorMemBlock;
		}

		MmappedMemBlock* ObtainPrefabMemBlock(size_t size, GameObject* &ret_entryPointRaw, const char* description);
	};


}

#define ALLOCATE 				GetObject().MEMORY.allocateSystemInBlock
#define DEALLOCATE 				GetObject().MEMORY.deallocateSystemInBlock
#define SYSTEMS_ALLOCATOR 		GetObject().MEMORY.GetSystemsAllocator()
#define STD_ALLOCATOR 			GetObject().MEMORY.GetStdAllocator()

#endif