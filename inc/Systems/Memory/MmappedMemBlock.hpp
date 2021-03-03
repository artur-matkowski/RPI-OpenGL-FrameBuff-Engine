#ifndef H_MmappedMemBlock
#define H_MmappedMemBlock
#include <bitforge/utils/bfu.hpp>
#include <SharedPtr.hpp>

namespace asapi
{
	class MemoryManagmentSystem;

	class MmappedMemBlock: public bfu::MemBlockBase
	{
		//we need to make it friend to allow stack initialization, and we need stack initialziation becouse we need to have it available ASAP
		friend MemoryManagmentSystem;

	protected:
		SharedPtr<void*>			m_buffStartPtr 		= nullptr;
		SharedPtr<void*> 			m_buffFreePtr 		= nullptr;
		SharedPtr<int> 				m_selfRefCounter 	= nullptr;
		SharedPtr<void*> 			m_buffEndPtr 		= nullptr;
		size_t 						m_deallocatedMemory = 0;
		static void* 				s_unclaimedMemPtr;

		MmappedMemBlock(const char* blockName, size_t size = 0);
		MmappedMemBlock(const char* name);
		MmappedMemBlock(const MmappedMemBlock& cp);


	public:
		static size_t PageSize();

		~MmappedMemBlock();

		static MmappedMemBlock* InitNoFile(	 const char* 	blockName, size_t size = 0);
		static MmappedMemBlock* InitFileRead(	 const char* 	blockName);
		static MmappedMemBlock* InitFileWrite(	 const char* 	blockName);
		
		void Resize( size_t 		newSize);
		
		void Dispouse();




		virtual void* 	allocate(	int 			elements
									,std::size_t 	sizeOf
									,std::size_t	alignOf		) override;


		virtual void 	deallocate(	void* 			ptr
									,std::size_t 	size 		) override;


		void free()						{ *m_buffFreePtr = m_buffStartPtr;							}

		size_t getFreeMemory() 			{ return (size_t)*m_buffEndPtr  - (size_t)*m_buffFreePtr; 	}
		size_t getUsedMemory() 			{ return (size_t)*m_buffFreePtr - (size_t)m_buffStartPtr;	}
		size_t size()					{ return (size_t)*m_buffEndPtr  - (size_t)m_buffStartPtr;	}

		void* end()						{ return *m_buffEndPtr;										}
		void* getRefPtr()				{ return this;												}
		void* getMemPtr()				{ return m_buffStartPtr; 									}

		virtual bool owns(void* ptr)	{ return ((size_t)m_buffStartPtr<(size_t)ptr && (size_t)ptr < (size_t)*m_buffEndPtr); }
	};
}

#endif
