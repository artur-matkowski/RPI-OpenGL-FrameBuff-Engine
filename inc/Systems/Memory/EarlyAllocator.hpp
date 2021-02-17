#ifndef H_EarlyAllocator
#define H_EarlyAllocator 
#include <climits>
#include <cstdlib>

namespace asapgl
{
	template <class T>
	struct EarlyAllocator  {

		typedef T value_type;


		EarlyAllocator() noexcept 
		{
		}
		~EarlyAllocator() noexcept 
		{
		}

		template <class U> EarlyAllocator (const EarlyAllocator<U>& cp) noexcept 
		{
	  	}

		T* allocate (std::size_t elements) 
		{	
			return (T*)aligned_alloc(alignof(T), sizeof(T) * elements);
		}

		void deallocate (T* p, std::size_t n) 
		{	
			free(p);
		}



	};

	template <class T, class U>
	bool operator==(const EarlyAllocator<T>&, const EarlyAllocator<U>&) { return true; }
	template <class T, class U>
	bool operator!=(const EarlyAllocator<T>&, const EarlyAllocator<U>&) { return false; }
}

#endif