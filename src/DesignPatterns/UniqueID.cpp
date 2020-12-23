#include "UniqueID.hpp"
#include <stdlib.h>
#include <time.h> 

namespace asapgl
{
	UniqueID::UniqueID()
			:bfu::SerializableClassBase()
			,ID("ID",this)
	{
		ID32[0] = time(NULL);
		ID32[1] = rand();
		ID = ID64;
	}


	void UniqueID::Serialize(bfu::JSONStream& stream)
	{
		ID = ID64;
		bfu::SerializableClassBase::Serialize(stream);
	}

	void UniqueID::Deserialize(bfu::JSONStream& stream)
	{
		bfu::SerializableClassBase::Deserialize(stream);
		ID64 = ID;
	}
}