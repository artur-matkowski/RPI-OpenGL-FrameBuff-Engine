#include "PrefabMemBlock.hpp"
#include "Systems.hpp"
#include <unistd.h>
#include <sys/mman.h>
#include <memory>
#include "GameObject.hpp"


namespace asapi
{
	size_t PrefabMemBlock::PageSize()
	{
		static size_t pageSize = sysconf(_SC_PAGE_SIZE);
	    return pageSize;
	}
	void* PrefabMemBlock::s_unclaimedMemPtr = (void*)PageSize();

	PrefabMemBlock::PrefabMemBlock(const char* name)
		:MemBlockBase(name)
		,m_buffFreePtr()
		,m_selfRefCounter()
		,m_buffEndPtr()
	{}

	PrefabMemBlock::PrefabMemBlock(const char* name, size_t size)
		:MemBlockBase(name)
		,m_buffFreePtr()
		,m_selfRefCounter()
		,m_buffEndPtr()
	{
		if(size==0) size = PageSize();

		void* ptr = mmap(s_unclaimedMemPtr, size, 
                PROT_READ | PROT_WRITE, 
                MAP_PRIVATE | MAP_ANONYMOUS, 
                -1, 0);

		std::memset(ptr, 0, size);

		//We are going to build our sharedPtrs inside our own managed memory, We asume a firstly builded object will be the last to go.

		void* freePtr = ptr;
		int* i_ptr1 = nullptr;
		int* i_ptr2 = nullptr;
		int* i_ptr3 = nullptr;
		int* i_ptr4 = nullptr;
		void** v_ptr1 = nullptr;
		void** v_ptr2 = nullptr;
		int*  i_ptr33 = nullptr;
		void** v_ptr4 = nullptr;

		if( nullptr != std::align(alignof(int), sizeof(int), freePtr, size ) )
		{
			i_ptr1 = (int*)freePtr;
			freePtr = (void*) ((size_t)freePtr + sizeof(int));
		} else { log::error << "Can not build PrefabMemBlock" << std::endl; }


		if( nullptr != std::align(alignof(int), sizeof(int), freePtr, size ) )
		{
			i_ptr2 = (int*)freePtr;
			freePtr = (void*) ((size_t)freePtr + sizeof(int));
		} else { log::error << "Can not build PrefabMemBlock" << std::endl; }


		if( nullptr != std::align(alignof(int), sizeof(int), freePtr, size ) )
		{
			i_ptr3 = (int*)freePtr;
			freePtr = (void*) ((size_t)freePtr + sizeof(int));
		} else { log::error << "Can not build PrefabMemBlock" << std::endl; }


		if( nullptr != std::align(alignof(int), sizeof(int), freePtr, size ) )
		{
			i_ptr4 = (int*)freePtr;
			freePtr = (void*) ((size_t)freePtr + sizeof(int));
		} else { log::error << "Can not build PrefabMemBlock" << std::endl; }


		if( nullptr != std::align(alignof(void*), sizeof(void*), freePtr, size ) )
		{
			v_ptr1 = (void**)freePtr;
			freePtr = (void*) ((size_t)freePtr + sizeof(void*));
		} else { log::error << "Can not build PrefabMemBlock" << std::endl; }

		if( nullptr != std::align(alignof(void*), sizeof(void*), freePtr, size ) )
		{
			v_ptr2 = (void**)freePtr;
			freePtr = (void*) ((size_t)freePtr + sizeof(void*));
		} else { log::error << "Can not build PrefabMemBlock" << std::endl; }

		if( nullptr != std::align(alignof(int), sizeof(int), freePtr, size ) )
		{
			i_ptr33= (int*)freePtr;
			freePtr = (void*) ((size_t)freePtr + sizeof(int));
		} else { log::error << "Can not build PrefabMemBlock" << std::endl; }

		if( nullptr != std::align(alignof(void*), sizeof(void*), freePtr, size ) )
		{
			v_ptr4 = (void**)freePtr;
			freePtr = (void*) ((size_t)freePtr + sizeof(void*));
		} else { log::error << "Can not build PrefabMemBlock" << std::endl; }


		new (&m_buffStartPtr) 	SharedPtr<void*>(v_ptr1, i_ptr1);
		new (&m_buffFreePtr)  	SharedPtr<void*>(v_ptr2, i_ptr2);
		new (&m_selfRefCounter) SharedPtr<int>  (i_ptr33,i_ptr3);
		new (&m_buffEndPtr) 	SharedPtr<void*>(v_ptr4, i_ptr4);


		*m_buffStartPtr			= ptr;
		*m_buffFreePtr			= freePtr;
		*m_selfRefCounter		= 1;
		*m_buffEndPtr			= (void*)((size_t)ptr + size);

		m_prefabEntryVector = (GameObject*) this->allocate(1, sizeof(GameObject), alignof(GameObject));

		//SYSTEMS::GetObject().MEMORY.RegisterMemBlock( this );
	}
	PrefabMemBlock::PrefabMemBlock(const PrefabMemBlock& cp)
		:MemBlockBase(cp)
		,m_buffStartPtr(cp.m_buffStartPtr)
		,m_buffFreePtr(cp.m_buffFreePtr)
		,m_selfRefCounter(cp.m_selfRefCounter)
		,m_buffEndPtr(cp.m_buffEndPtr)
		,m_deallocatedMemory(cp.m_deallocatedMemory)
	{
		++(*m_selfRefCounter);
	}
	PrefabMemBlock::~PrefabMemBlock()
	{
		--(*m_selfRefCounter);
		if(*m_selfRefCounter==0)
			munmap(*m_buffFreePtr, (size_t)*m_buffEndPtr - (size_t)*m_buffStartPtr);
	}

	
	PrefabMemBlock* PrefabMemBlock::InitNoFile(	 const char* 	blockName, size_t size)
	{
		
		if(size==0) size = PageSize();

		void* ptr = mmap(s_unclaimedMemPtr, size, 
                PROT_READ | PROT_WRITE, 
                MAP_PRIVATE | MAP_ANONYMOUS, 
                -1, 0);

		std::memset(ptr, 0, size);

		//We are going to build our sharedPtrs inside our own managed memory, We asume a firstly builded object will be the last to go.

		void* freePtr = ptr;
		int* i_ptr1 = nullptr;
		int* i_ptr2 = nullptr;
		int* i_ptr3 = nullptr;
		int* i_ptr4 = nullptr;
		void** v_ptr1 = nullptr;
		void** v_ptr2 = nullptr;
		int*  i_ptr33 = nullptr;
		void** v_ptr4 = nullptr;
		PrefabMemBlock* mmb = nullptr;

		if( nullptr != std::align(alignof(PrefabMemBlock), sizeof(PrefabMemBlock), freePtr, size ) )
		{
			mmb = (PrefabMemBlock*)freePtr;
			freePtr = (void*) ((size_t)freePtr + sizeof(PrefabMemBlock));
		} else { log::error << "Can not build PrefabMemBlock" << std::endl; }


		if( nullptr != std::align(alignof(int), sizeof(int), freePtr, size ) )
		{
			i_ptr1 = (int*)freePtr;
			freePtr = (void*) ((size_t)freePtr + sizeof(int));
		} else { log::error << "Can not build PrefabMemBlock" << std::endl; }


		if( nullptr != std::align(alignof(int), sizeof(int), freePtr, size ) )
		{
			i_ptr2 = (int*)freePtr;
			freePtr = (void*) ((size_t)freePtr + sizeof(int));
		} else { log::error << "Can not build PrefabMemBlock" << std::endl; }


		if( nullptr != std::align(alignof(int), sizeof(int), freePtr, size ) )
		{
			i_ptr3 = (int*)freePtr;
			freePtr = (void*) ((size_t)freePtr + sizeof(int));
		} else { log::error << "Can not build PrefabMemBlock" << std::endl; }


		if( nullptr != std::align(alignof(int), sizeof(int), freePtr, size ) )
		{
			i_ptr4 = (int*)freePtr;
			freePtr = (void*) ((size_t)freePtr + sizeof(int));
		} else { log::error << "Can not build PrefabMemBlock" << std::endl; }


		if( nullptr != std::align(alignof(void*), sizeof(void*), freePtr, size ) )
		{
			v_ptr1 = (void**)freePtr;
			freePtr = (void*) ((size_t)freePtr + sizeof(void*));
		} else { log::error << "Can not build PrefabMemBlock" << std::endl; }

		if( nullptr != std::align(alignof(void*), sizeof(void*), freePtr, size ) )
		{
			v_ptr2 = (void**)freePtr;
			freePtr = (void*) ((size_t)freePtr + sizeof(void*));
		} else { log::error << "Can not build PrefabMemBlock" << std::endl; }

		if( nullptr != std::align(alignof(int), sizeof(int), freePtr, size ) )
		{
			i_ptr33= (int*)freePtr;
			freePtr = (void*) ((size_t)freePtr + sizeof(int));
		} else { log::error << "Can not build PrefabMemBlock" << std::endl; }

		if( nullptr != std::align(alignof(void*), sizeof(void*), freePtr, size ) )
		{
			v_ptr4 = (void**)freePtr;
			freePtr = (void*) ((size_t)freePtr + sizeof(void*));
		} else { log::error << "Can not build PrefabMemBlock" << std::endl; }


		new (&mmb->m_buffStartPtr) 		SharedPtr<void*>(v_ptr1, i_ptr1);
		new (&mmb->m_buffFreePtr)  		SharedPtr<void*>(v_ptr2, i_ptr2);
		new (&mmb->m_selfRefCounter) 	SharedPtr<int>  (i_ptr33,i_ptr3);
		new (&mmb->m_buffEndPtr) 		SharedPtr<void*>(v_ptr4, i_ptr4);


		*mmb->m_buffStartPtr		= ptr;
		*mmb->m_buffFreePtr			= freePtr;
		*mmb->m_selfRefCounter		= 1;
		*mmb->m_buffEndPtr			= (void*)((size_t)ptr + size);

		mmb->m_prefabEntryVector = (GameObject*) mmb->allocate(1, sizeof(GameObject), alignof(GameObject));

		SYSTEMS::GetObject().MEMORY.RegisterMemBlock( mmb );

		return mmb;
	}
	PrefabMemBlock* PrefabMemBlock::InitFileRead(	 const char* 	blockName)
	{
		// new (this) PrefabMemBlock(blockName);

		// SYSTEMS::GetObject().MEMORY.RegisterMemBlock( this );
	}
	PrefabMemBlock* PrefabMemBlock::InitFileWrite(	 const char* 	blockName)
	{
		// new (this) PrefabMemBlock(blockName);

		// SYSTEMS::GetObject().MEMORY.RegisterMemBlock( this );
	}
		
	void PrefabMemBlock::Resize(size_t newSize)
	{
		// reqAddr = std::align(PageSize(), 1, reqAddr, size);
		// m_buffStartPtr = mmap(reqAddr, newSize-size(), 
  //               PROT_READ | PROT_WRITE, 
  //               MAP_PRIVATE | MAP_ANONYMOUS, 
  //               -1, 0);

		// //as we can not emlpace shared pointer we need temporary m_buffFreePtr to be able to later allocate m_selfRefCounter & m_buffFreePtr by custom_allocator<...>
		// //m_buffFreePtr = std::make_shared<void*>(m_buffStartPtr);
		// auto tmpStdMBlock = StdAllocatorMemBlock();
		// m_buffFreePtr = std::allocate_shared<void*>(custom_allocator<void*>( &tmpStdMBlock ), m_buffStartPtr);


		// std::memset(m_buffStartPtr, 0, size);
		// *m_buffEndPtr = (void*)((size_t)m_buffStartPtr + size);


		// m_selfRefCounter = std::allocate_shared<int>(custom_allocator<int>(this), 1);
		// m_buffFreePtr = std::allocate_shared<void*>(custom_allocator<void*>(this), *m_buffFreePtr);
	}
	void PrefabMemBlock::Dispouse()
	{
		SYSTEMS::GetObject().MEMORY.UnRegisterMemBlock( this );
		this->~PrefabMemBlock();
	}

	void* PrefabMemBlock::allocate (int elements, std::size_t sizeOf, std::size_t alignOf)
	{
		size_t size = getFreeMemory();

		void* tmp = *m_buffFreePtr;

		if ( *m_buffFreePtr = std::align(alignOf, sizeOf, *m_buffFreePtr, size ))
        {
            void* result = *m_buffFreePtr;
            size_t size = sizeOf * elements;
            size = size > 0 ? size : 1;
            *m_buffFreePtr = (void*)((size_t) *m_buffFreePtr + size);


            if(*m_buffFreePtr >= *m_buffEndPtr)
	        {
	            //std::cout << "Failed to allocate memory by PrefabMemBlock, requested size: " << sizeOf * elements << std::endl;
					//std::cout.flush();
					return nullptr;
	        }

			if(result == *m_buffFreePtr)
			{
				*m_buffFreePtr = (void*)((size_t)*m_buffFreePtr +1);
			}

			++m_allocationCount;

            return result;
        }
        return nullptr;
	}

	void PrefabMemBlock::deallocate (void* p, std::size_t n) 
	{
		m_deallocatedMemory += n;
		memset(p, 0, n);
		if( (size_t)p+n==(size_t)*m_buffFreePtr)
		{
            //std::cout << "Regaining memory becouse deallocate was called right after allocate on the same ptr" << std::endl;
			//std::cout.flush();
			*m_buffFreePtr = (void*)((size_t)*m_buffFreePtr - n);
		}

		++m_deallocationCount;
	}
}