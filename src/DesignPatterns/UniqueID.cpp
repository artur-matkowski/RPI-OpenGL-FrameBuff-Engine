#include "UniqueID.hpp"
#include <stdlib.h>
#include <time.h>
#include "Systems.hpp"

namespace asapi
{
	
	UniqueID::UniqueID( bfu::MemBlockBase* mBlock )
		:bfu::SerializableClassBase(mBlock)
		,m_ID("m_ID", this)
	{
		auto &TIME = SYSTEMS::GetObject().TIME;

		ID32[0] = TIME.SecondsSince1970();
		ID32[1] = TIME.RANDOM.UINT32();
		
		m_ID = ID64;
	}
	UniqueID::UniqueID( const UniqueID& cp )
		:bfu::SerializableClassBase(cp)
		,m_ID("m_ID", this)
	{
		ID64 = cp.ID64;
	}


	// void UniqueID::PreSerializationCallback()
	// {
	// 	m_ID = ID64;
	// }
	// void UniqueID::PostDeserializationCallback()
	// {
	// 	ID64 = m_ID;
	// }
}