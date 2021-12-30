#ifndef H_BinaryResourceTracker
#define H_BinaryResourceTracker
#include "bfu.hpp"
#include "UniqueID.hpp"


using bfu::string;


namespace asapi
{
	class BinaryResourceTracker: public bfu::SerializableClassBase<BinaryResourceTracker>
	{
		static std::string s_projectPath;
		SERIALIZABLE_VAR( BinaryResourceTracker, string, m_displayedName );

		UniqueID m_binaryResourceID;
		UniqueID m_resourceTrackerID;
		const char* m_resourceTypeExtension = 0; //that is a pointer to static const char* of TextProcessor class

	public:
		int ResourceReferenceCounter = 0;

		BinaryResourceTracker(const UniqueID& resourceTrackerID
							, const UniqueID& subResourceID
							, const std::string& displayName
							, const char* resourceTypeExtension 
							)
			:m_binaryResourceID(subResourceID)
			,m_resourceTrackerID(resourceTrackerID)
			,m_displayedName(displayName)
			,m_resourceTypeExtension(resourceTypeExtension)
		{
			TryDeserialize();
		};
		BinaryResourceTracker(){};
		~BinaryResourceTracker()
		{}

		inline UniqueID GetResourceID() const 		{ return m_resourceTrackerID; }
		inline UniqueID GetSubResourceID() const 	{ return m_binaryResourceID;  }
		inline const char* GetDisplayName() const	{ return m_displayedName.c_str(); }

						void SetDisplayName(const char* name);
		static inline 	void SetProjectPath(const char* path) { s_projectPath = path; }

		#ifdef IS_EDITOR
		bool OnGUI( UniqueID* out_newResource, const char* typeFilter )
		{
			//Render resource selector with type restriction
			//return true if resourcewas changes
			return false;
		}
		#endif

	protected:
		friend bfu::stream& operator<<(bfu::stream&, const BinaryResourceTracker& );
		void Serialize();
		bool TryDeserialize();
	};


	bfu::stream& operator<<(bfu::stream& st, const BinaryResourceTracker& obj);
}
#endif