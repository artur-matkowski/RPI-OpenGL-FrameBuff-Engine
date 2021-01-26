#ifndef _H_MemoryManagmentSystem
#define _H_MemoryManagmentSystem
#include <bitforge/utils/bfu.hpp>
#include <vector>

namespace asapgl
{
	class MemoryManagmentSystem
	{
		bfu::MonotonicMemBlock<1024*1024*10> 	SystemsMemoryBlock;
		bfu::operatorNEWstatistics				m_operatorNEWstatistics;
		bfu::StdAllocatorMemBlock				m_StdAllocatorMemBlock;

		std::vector<bfu::MemBlockBase*, bfu::custom_allocator<bfu::MemBlockBase*>> v_memBlocks;

	public:
		MemoryManagmentSystem()
			:SystemsMemoryBlock("SystemsMemoryBlock")
			,v_memBlocks(bfu::custom_allocator<bfu::MemBlockBase*>(&SystemsMemoryBlock))
		{
			v_memBlocks.reserve(16);
			v_memBlocks.push_back(&SystemsMemoryBlock);
			v_memBlocks.push_back(&m_operatorNEWstatistics);
			v_memBlocks.push_back(&m_StdAllocatorMemBlock);
		}


		void OnGUI();
	};
}

#endif