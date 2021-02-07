#include "UniqueID.hpp"
#include <stdlib.h>
#include <time.h>
#include "Systems.hpp"

namespace asapgl
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

	void UniqueID::Serialize(bfu::JSONStream& stream)
	{
		m_ID = ID64;
		stream << m_ID;
	}

	void UniqueID::Deserialize(bfu::JSONStream& stream)
	{
		stream >> m_ID;
		ID64 = m_ID;
	}
}