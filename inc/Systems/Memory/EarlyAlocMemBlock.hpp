#ifndef H_EarlyAlocMemBlock
#define H_EarlyAlocMemBlock
#include "Systems.hpp"
#include <bitforge/utils/bfu.hpp>


namespace asapi
{
	template <int stackSize>
	class EarlyAlocMemBlock: public bfu::MemBlockBase
	{
	protected:
		char m_buffStartPtr[stackSize];
		void* m_buffFreePtr = nullptr;
		void* m_buffEndPtr = nullptr;
		size_t m_deallocatedMemory = 0;

	public:
		EarlyAlocMemBlock(const char* name = "Early Allocator MemBlock")
			:MemBlockBase(name)
		{
			if( m_buffFreePtr == nullptr )
			{
				m_buffFreePtr = m_buffStartPtr;
				m_buffEndPtr = (void*)((size_t)m_buffStartPtr + (size_t)stackSize);
			}
		};



		~EarlyAlocMemBlock(){  };

		virtual void* allocate (int elements, std::size_t sizeOf, std::size_t alignOf)
		{
			if( m_buffFreePtr == nullptr )
				new (this) EarlyAlocMemBlock();

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
	            return result;
	        }
	        return nullptr;
		}

		virtual void deallocate (void* p, std::size_t n) 
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
		}

		void free()
		{
			m_buffFreePtr = m_buffStartPtr;
		}


		size_t getFreeMemory()
		{
			return (size_t)m_buffEndPtr- (size_t)m_buffFreePtr;
		}
		size_t getUsedMemory()
		{
			return (size_t)m_buffFreePtr- (size_t)m_buffStartPtr;
		}
		void* getRefPtr()
		{
			return this;
		}
		void* getMemPtr()
		{
			return m_buffStartPtr;
		}
		virtual bool owns(void* ptr)
		{
			if((size_t)m_buffStartPtr<(size_t)ptr && (size_t)ptr < (size_t)m_buffEndPtr)
				return true;
			
			return false;
		}


		static EarlyAlocMemBlock* GetMemBlock()
		{
			static EarlyAlocMemBlock<stackSize> _this;
			return &_this;
		}
	};
}

#endif