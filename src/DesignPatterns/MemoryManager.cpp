#include "MemoryManager.hpp"
#include <stdlib.h>
#include <iostream>

/*
void * operator new(size_t size)
{ 
	++alocations;
    std::cout << "Memory alocations:   " << alocations << 
    		   "\nMemory dealocations: " << dealocations << std::endl; 
    std::cout.flush();
    void * p = malloc(size); 
    return p; 
} 
  
void operator delete(void * p) noexcept
{ 
	++dealocations;
    std::cout << "Memory alocations:   " << alocations << 
    		   "\nMemory dealocations: " << dealocations << std::endl; 
    std::cout.flush();
    free(p); 
}
*/