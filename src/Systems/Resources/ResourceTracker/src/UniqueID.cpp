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
		m_ID = cp.ID64;
	}
	UniqueID::UniqueID( const uint64_t& value )
	{
		ID64 = value;
		m_ID = value;
	}


	UniqueID::UniqueID( UniqueID&& cp ) noexcept
	{
		m_ID = cp.m_ID;
		ID64 = cp.ID64;

		cp.m_ID = 0;
		cp.ID64 = 0;
	}

	UniqueID& UniqueID::operator=(UniqueID&& cp)
	{
		m_ID = cp.m_ID;
		ID64 = cp.ID64;

		cp.m_ID = 0;
		cp.ID64 = 0;

		return *this;
	}
	
	bool UniqueID::operator<( const UniqueID& other )
	{
		return ID64 < other.ID64;
	}

	bool UniqueID::operator>( const UniqueID& other )
	{
		return ID64 > other.ID64;
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