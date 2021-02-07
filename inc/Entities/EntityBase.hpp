#ifndef _H_EntityBase
#define _H_EntityBase
#include "UniqueID.hpp"
#include "object.hpp"

namespace asapgl
{
	class EntityBase: public object, public bfu::SerializableClassBase
	{
	protected:
		//SERIALIZABLE_VAR( UniqueID ) m_ID;

	public:
		EntityBase( bfu::MemBlockBase* mBlock )
			:bfu::SerializableClassBase(mBlock)
		{};
		EntityBase( const EntityBase& cp )
			:bfu::SerializableClassBase(cp.m_mBlock)
		{};
		~EntityBase(){};

		bfu::MemBlockBase* GetMemBlock()
		{
			return m_mBlock;
		}
	};
}

#endif