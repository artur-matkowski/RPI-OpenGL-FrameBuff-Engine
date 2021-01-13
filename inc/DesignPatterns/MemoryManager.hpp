#ifndef _H_MemoryManager
#define _H_MemoryManager
#include <cstddef>


static int alocations = 0;
static int dealocations = 0;

void * operator new(size_t size);
void operator delete(void * p) noexcept;



#endif