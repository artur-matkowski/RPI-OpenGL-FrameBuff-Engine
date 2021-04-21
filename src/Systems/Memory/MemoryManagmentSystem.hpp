#ifndef _H_MemoryManagmentSystem
#define _H_MemoryManagmentSystem
#include <bitforge/utils/bfu.hpp>
#include <vector>
#include "object.hpp"


namespace asapi
{
	class GameObject;
	
	class MemoryManagmentSystem
	{
	protected:
		char buff[1024*1024*10];

		bfu::PreAllocatedMemBlock 					SystemsMemoryBlock;
		bfu::StdAllocatorMemBlock					m_StdAllocatorMemBlock;

		std::vector<bfu::MemBlockBase*, bfu::custom_allocator<bfu::MemBlockBase*>> v_memBlocks;

	public:

		MemoryManagmentSystem()
			:SystemsMemoryBlock(buff, 1024*1024*10, "Systems Memory Block")
			,v_memBlocks(bfu::custom_allocator<bfu::MemBlockBase*>(&SystemsMemoryBlock))
			,m_StdAllocatorMemBlock("std allocator")
		{
			v_memBlocks.reserve(16);
			v_memBlocks.push_back(&SystemsMemoryBlock);
			v_memBlocks.push_back(&m_StdAllocatorMemBlock);

			SetNewAllocator(&m_StdAllocatorMemBlock);
		}

		~MemoryManagmentSystem()
		{
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

		inline bfu::MemBlockBase* GetSystemsAllocator()
		{
			return &SystemsMemoryBlock;
		}

		bfu::MemBlockBase* RequestPrefabMemBlock(const char* memblockDesc);
		void ReleasePrefabMemBlock(bfu::MemBlockBase* memblock);

		inline bfu::MemBlockBase* GetSTDAllocator()
		{
			return &m_StdAllocatorMemBlock;
		}
	};


}

#define ALLOCATE 				GetObject().MEMORY.allocateSystemInBlock
#define DEALLOCATE 				GetObject().MEMORY.deallocateSystemInBlock
#define DEALLOCATE_GLOBAL(x)	bfu::MemBlockBase::DeallocateUnknown(x)
#define SYSTEMS_ALLOCATOR 		GetObject().MEMORY.GetSystemsAllocator()

#define STD_NEW(x,t) 			(t*)SYSTEMS::GetObject().MEMORY.GetSTDAllocator()->allocate(x, sizeof(t), alignof(t))

#endif