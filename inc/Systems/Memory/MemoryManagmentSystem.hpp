#ifndef _H_MemoryManagmentSystem
#define _H_MemoryManagmentSystem
#include <bitforge/utils/bfu.hpp>
#include <vector>
#include "object.hpp"
#include "PrefabMemBlock.hpp"
#include "StaticAllocatorMemBlock.hpp"


namespace asapi
{
	class GameObject;
	
	class MemoryManagmentSystem
	{
	protected:
		char buff[1024*1024*10];

		StaticAllocatorMemBlock 				SystemsMemoryBlock;
		//bfu::MallocAllocator					m_operatorNEWstatistics;
		bfu::StdAllocatorMemBlock				m_StdAllocatorMemBlock;
		#ifdef IS_EDITOR
		bfu::StdAllocatorMemBlock					m_GUIAllocatorMemBlock;
		#endif
		bfu::StdAllocatorMemBlock 					m_JSONAllocator;

		std::vector<bfu::MemBlockBase*, bfu::custom_allocator<bfu::MemBlockBase*>> v_memBlocks;

		// MmappedMemBlock* 						p_memBlockCache = 0;
		// void* 									p_memBlocksEnd = 0;

	public:

		MemoryManagmentSystem()
			:SystemsMemoryBlock(buff, 1024*1024*10, "Systems Memory Block")
			,v_memBlocks(bfu::custom_allocator<bfu::MemBlockBase*>(&SystemsMemoryBlock))
			,m_StdAllocatorMemBlock("Late [operator new()]\nallocator (malloc)")
			#ifdef IS_EDITOR
			,m_GUIAllocatorMemBlock("GUI Allocator (malloc)")
			#endif
			,m_JSONAllocator("JSON Allocator (malloc)")
		{
			v_memBlocks.reserve(16);
			v_memBlocks.push_back(&SystemsMemoryBlock);
			//v_memBlocks.push_back(&m_operatorNEWstatistics);
			v_memBlocks.push_back(&m_StdAllocatorMemBlock);
			#ifdef IS_EDITOR
			v_memBlocks.push_back(&m_GUIAllocatorMemBlock);
			#endif
			v_memBlocks.push_back(&m_JSONAllocator);

			auto oldEarlyAllocator = SetNewAllocator(&m_StdAllocatorMemBlock);

			v_memBlocks.push_back( oldEarlyAllocator );
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

		inline bfu::StdAllocatorMemBlock* GetStdAllocator()
		{
			return &m_StdAllocatorMemBlock;
		}
		#ifdef IS_EDITOR
		inline bfu::MemBlockBase* GetGUIAllocator()
		{
			return &m_GUIAllocatorMemBlock;
		}
		#endif

		inline bfu::MemBlockBase* GetJSONAllocator()
		{
			return &m_JSONAllocator;
		}

		bfu::MemBlockBase* RequestPrefabMemBlock(const char* memblockDesc);
		void ReleasePrefabMemBlock(bfu::MemBlockBase* memblock);
	};


}

#define ALLOCATE 				GetObject().MEMORY.allocateSystemInBlock
#define DEALLOCATE 				GetObject().MEMORY.deallocateSystemInBlock
#define DEALLOCATE_GLOBAL(x)	bfu::MemBlockBase::DeallocateUnknown(x)
#define SYSTEMS_ALLOCATOR 		GetObject().MEMORY.GetSystemsAllocator()
#define STD_ALLOCATOR 			GetObject().MEMORY.GetStdAllocator()
#ifdef IS_EDITOR
#define GUI_ALLOCATOR 			GetObject().MEMORY.GetGUIAllocator()
#endif
#define JSON_ALLOCATOR 			GetObject().MEMORY.GetJSONAllocator()


#endif