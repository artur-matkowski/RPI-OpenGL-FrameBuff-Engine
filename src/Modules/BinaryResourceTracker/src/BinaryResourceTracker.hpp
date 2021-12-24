#ifndef H_BinaryResourceTracker
#define H_BinaryResourceTracker
#include "bfu.hpp"
#include "UniqueID.hpp"


using bfu::string;


namespace asapi
{

	class BinaryResourceTracker: public bfu::SerializableClassBase<BinaryResourceTracker>
	{
#ifdef TESTS
	public:
#endif		
		SERIALIZABLE_OBJ( BinaryResourceTracker, UniqueID, m_binaryResourceID );
		SERIALIZABLE_OBJ( BinaryResourceTracker, UniqueID, m_resourceTrackerID );
		SERIALIZABLE_VAR( BinaryResourceTracker, string, m_displayedName );

		int ResourceReferenceCounter;
	public:
		BinaryResourceTracker(){};
		~BinaryResourceTracker()
		{}

		#ifdef IS_EDITOR
		bool OnGUI( UniqueID* out_newResource, const char* typeFilter )
		{
			//Render resource selector with type restriction
			//return true if resourcewas changes
			return false;
		}
		#endif
	};

}
#endif