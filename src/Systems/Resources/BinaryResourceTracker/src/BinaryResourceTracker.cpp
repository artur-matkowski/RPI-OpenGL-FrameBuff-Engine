#include "BinaryResourceTracker.hpp"
#include "ResourceTracker.hpp"
#include "bfu.hpp"
#include "File.hpp"

namespace asapi
{
	std::string BinaryResourceTracker::s_projectPath;


	void BinaryResourceTracker::Serialize()
	{
		bfu::JSONSerializer json;
		FILE::STREAM file;
		std::string filepath = s_projectPath 
								+ RESOURCE_BINARIES_DIR "/" 
								+ std::to_string( GetSubResourceID().ID() ) 
								+ m_resourceTypeExtension
								+ ".json";

		json.Serialize( this );

		file.InitForWrite( filepath.c_str() );
		file.Write( json.c_str(), json.size() );

		
		file.Close();
	}

	bool BinaryResourceTracker::TryDeserialize()
	{
		bfu::JSONSerializer json;
		FILE::MMAP file;
		std::string filepath = s_projectPath 
								+ RESOURCE_BINARIES_DIR "/" 
								+ std::to_string( GetSubResourceID().ID() ) 
								+ m_resourceTypeExtension
								+ ".json";


		if( !file.TryInitForRead( filepath.c_str() ) )
		{
			return false;
		}

		json.assignData( (char*)file.Data(), file.Size() );
		json.Deserialize( this );
		file.Close();
		
		return true;
	}


	void BinaryResourceTracker::SetDisplayName(const char* name)
	{
		m_displayedName = name;
		Serialize();
	}


	bfu::stream& operator<<(bfu::stream& st, const BinaryResourceTracker& obj)
	{
		st << "\n\t\tBinary resource ID  : " << std::to_string( obj.m_binaryResourceID.ID() ).c_str();
		st << "\n\t\tResource Tracker ID : " << std::to_string( obj.m_resourceTrackerID.ID() ).c_str();
		st << "\n\t\tDisplayed Name      : " << obj.m_displayedName.c_str();

		return st;
	}
}