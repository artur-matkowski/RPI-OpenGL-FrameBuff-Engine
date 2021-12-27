#include "BinaryResourceTracker.hpp"


namespace asapi
{
	bfu::stream& operator<<(bfu::stream& st, const BinaryResourceTracker& obj)
	{
		st << "\n\t\tBinary resource ID  : " << std::to_string( obj.m_binaryResourceID.ID() ).c_str();
		st << "\n\t\tResource Tracker ID : " << std::to_string( obj.m_resourceTrackerID.ID() ).c_str();
		st << "\n\t\tDisplayed Name      : " << obj.m_displayedName.c_str();

		return st;
	}
}