#ifndef _H_EntityBase
#define _H_EntityBase
#include "UniqueID.hpp"
#include "object.hpp"

namespace asapi
{
	class EntityBase: public object//, public bfu::SerializableClassBase
	{
	protected:
		//bfu::SerializableVar<UniqueID> m_ID;
		bfu::MemBlockBase* m_mBlock;

	public:
		EntityBase( bfu::MemBlockBase* mBlock )
			:m_mBlock(mBlock)
			//,m_ID("m_ID", this, mBlock)
		{};
		EntityBase( const EntityBase& cp )
			:m_mBlock(cp.m_mBlock)
			//,m_ID("m_ID", this, cp.m_mBlock)
		{};
		~EntityBase(){};

		bfu::MemBlockBase* GetMemBlock()
		{
			return m_mBlock;
		}
	};
}

#endif