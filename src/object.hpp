#ifndef _H_OBJECT
#define _H_OBJECT
#include "bfu.hpp"
#include "SharedPtr.hpp"
#include "glm.hpp"
#include "Profiler.hpp"
#include <cstdlib>

//	Allowed #defines to controll build process
//	#define IS_EDITOR
//	#define IS_PLAYER
//	#define IS_TARGET

//temporary solution
// #ifndef IS_PLAYER
// 	#define IS_EDITOR
// #endif


#ifdef IS_EDITOR
	#ifdef IS_PLAYER
		#error "Both IS_EDITOR & IS_PLAYER defined, make up your mind ;)"
	#endif
#endif

#ifndef IS_EDITOR
	#ifndef IS_PLAYER
		#error "Neither IS_EDITOR, nor IS_PLAYER defined, make up your mind ;)"
	#endif
#endif

namespace asapi
{
	class object{};
}

#ifdef IS_EDITOR
	#define ASSERT(b, msg) if(b) log::error << msg << std::endl
#else
	#define ASSERT(b, msg)
#endif


void* operator new(std::size_t size);
void operator delete(void* p) noexcept;
void SetNewAllocator(bfu::MemBlockBase* allocator);

#define PROTECTED(x) protected: x; public:
#define MAX_PATH_SIZE 1024
#define MAX_FILENAME_SIZE 256

#endif
