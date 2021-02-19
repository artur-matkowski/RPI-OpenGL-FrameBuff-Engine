#include "MmappedMemBlock.hpp"
#include "Systems.hpp"

namespace asapi
{
	MmappedMemBlock::MmappedMemBlock(void* reqAddr, size_t size, const char* name)
		:bfu::MmappedMemBlock(reqAddr, size, name)
	{
		SYSTEMS::GetObject().MEMORY.RegisterMemBlock( this );
	}
	MmappedMemBlock::MmappedMemBlock(const MmappedMemBlock& cp)
		:bfu::MmappedMemBlock(cp)
	{

	}
	MmappedMemBlock::~MmappedMemBlock()
	{

	}

	void MmappedMemBlock::Init(void* reqAddr, size_t size, const char* name)
	{
		new (this) MmappedMemBlock(reqAddr, size, name);
	}
	void MmappedMemBlock::Dispouse()
	{
		this->~MmappedMemBlock();
	}
}