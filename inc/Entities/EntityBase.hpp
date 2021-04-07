#ifndef _H_EntityBase
#define _H_EntityBase
#include "UniqueID.hpp"
#include "object.hpp"

namespace asapi
{

	class EntityInterface: public object
	{
	protected:
		bfu::MemBlockBase* m_mBlock;

	public:
		EntityInterface( bfu::MemBlockBase* mBlock )
			:m_mBlock(mBlock)
		{};
		EntityInterface( const EntityInterface& cp ) = delete;

		~EntityInterface(){};

		bfu::MemBlockBase* GetMemBlock()
		{
			return m_mBlock;
		}
	};


	template<class T>
	class EntityBase: public EntityInterface, public bfu::SerializableClassBase<T>
	{
	protected:
		
	public:
		EntityBase( bfu::MemBlockBase* mBlock )
			:EntityInterface(mBlock)
		{};
		EntityBase( const EntityBase<T>& cp )
			:EntityInterface(cp.m_mBlock)
		{};
		~EntityBase(){};
	};
}

#endif