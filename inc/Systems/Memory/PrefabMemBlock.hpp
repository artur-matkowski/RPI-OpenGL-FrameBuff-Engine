#ifndef H_PrefabMemBlock
#define H_PrefabMemBlock
#include <bitforge/utils/bfu.hpp>
#include "SharedPtr.hpp"

namespace asapi
{
	class MemoryManagmentSystem;
	class GameObject;

	class PrefabMemBlock: public bfu::MemBlockBase
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

		PrefabMemBlock(const char* blockName, GameObject* entryVectorOwner, size_t size);
		PrefabMemBlock(const char* name);


	public:
		PrefabMemBlock(const PrefabMemBlock& cp);
		
		static size_t PageSize();

		~PrefabMemBlock();

		static PrefabMemBlock* InitNoFile(	 const char* 	blockName, GameObject* entryVectorOwner, size_t size = 0);
		static PrefabMemBlock* InitFileRead(	 const char* 	blockName, GameObject* entryVectorOwner);
		static PrefabMemBlock* InitFileWrite(	 const char* 	blockName, GameObject* entryVectorOwner);
		
		void Resize( size_t 		newSize = 0);
		
		void ForceDispouse();



		virtual void* 	allocate(	int 			elements
									,std::size_t 	sizeOf
									,std::size_t	alignOf		) override;


		virtual void 	deallocate(	void* 			ptr
									,std::size_t 	size 		) override;


		void free()						{ *m_buffFreePtr = *m_buffStartPtr;							}

		size_t getFreeMemory() 			{ return (size_t)*m_buffEndPtr  - (size_t)*m_buffFreePtr; 	}
		size_t getUsedMemory() 			{ return (size_t)*m_buffFreePtr - (size_t)*m_buffStartPtr;	}
		size_t size()					{ return (size_t)*m_buffEndPtr  - (size_t)*m_buffStartPtr;	}

		void* end()						{ return *m_buffEndPtr;										}
		void* getRefPtr()				{ return this;												}
		void* getMemPtr()				{ return *m_buffStartPtr; 									}

		virtual bool owns(void* ptr)	{ return ((size_t)*m_buffStartPtr<(size_t)ptr && (size_t)ptr < (size_t)*m_buffEndPtr); }
	};
}

#endif
