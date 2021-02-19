#ifndef H_MmappedMemBlock
#define H_MmappedMemBlock
#include <bitforge/utils/bfu.hpp>

namespace asapi
{
	class MemoryManagmentSystem;
	class MmappedMemBlock: public bfu::MmappedMemBlock 
	{
		//we need to make it friend to allow stack initialization, and we need stack initialziation becouse we need to have it available ASAP
		friend MemoryManagmentSystem;

		MmappedMemBlock(void* reqAddr, size_t size, const char* name);
		MmappedMemBlock(const MmappedMemBlock& cp);

	public:
		~MmappedMemBlock();

		void Init(void* reqAddr, size_t size, const char* name = "MmappedMemBlock");
		void Dispouse();		
	};
}

#endif
