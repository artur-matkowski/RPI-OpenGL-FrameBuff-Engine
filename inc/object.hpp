#ifndef _H_OBJECT
#define _H_OBJECT
#include "SharedPtr.hpp"
#include "glm.hpp"
#include <cstdlib>

//	Allowed #defines to controll build process
//	#define IS_EDITOR
//	#define IS_PLAYER
//	#define IS_TARGET

//temporary solution
// #ifndef IS_PLAYER
// 	#define IS_EDITOR
// #endif

#ifdef IS_TARGET
	#ifdef IS_EDITOR
		#define USE_XLIB
	#endif
#else
	#define USE_XLIB
#endif

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


void* operator new(std::size_t size);
void operator delete(void* p) noexcept;


#endif
