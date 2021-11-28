#include "UniqueID.hpp"
#include <stdlib.h>
#include <time.h>

namespace asapi
{
	
	UniqueID::UniqueID()
	{
		ID32[0] = time(NULL);
		ID32[1] = rand();
		
		m_ID = ID64;
	}
	UniqueID::UniqueID( const UniqueID& cp )
	{
		ID64 = cp.ID64;
	}


	void UniqueID::PreSerializationCallback()
	{
		m_ID = ID64;
	}
	void UniqueID::PostDeserializationCallback()
	{
		ID64 = m_ID;
	}
}