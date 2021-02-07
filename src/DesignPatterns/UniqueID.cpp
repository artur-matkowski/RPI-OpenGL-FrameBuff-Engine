#include "UniqueID.hpp"
#include <stdlib.h>
#include <time.h> 

namespace asapgl
{
	
	UniqueID::UniqueID( bfu::MemBlockBase* mBlock )
			:bfu::SerializableClassBase(mBlock)
			,ID("ID", this)
	{
		ID32[0] = time(NULL);
		ID32[1] = rand();
		ID = ID64;
	}

	void UniqueID::Serialize(bfu::JSONStream& stream)
	{
		ID = ID64;
		stream << ID;
	}

	void UniqueID::Deserialize(bfu::JSONStream& stream)
	{
		stream >> ID;
		ID64 = ID;
	}
}