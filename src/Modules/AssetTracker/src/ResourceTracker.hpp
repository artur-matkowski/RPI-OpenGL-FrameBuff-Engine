#ifndef H_RESOURCETRACKET
#define H_RESOURCETRACKET
#include "bfu.hpp"
#include "File.hpp"
#include "UniqueID.hpp"
//resource is a source file like: jpg, glsl, psd, fbx, txt, etc. 

using std::string;

namespace asapi
{
	class ResourceTracker: public bfu::SerializableClassBase<ResourceTracker>
	{
		SERIALIZABLE_OBJ( ResourceTracker, UniqueID, m_resourceID );
		SERIALIZABLE_VAR( ResourceTracker, string, m_filename );
		SERIALIZABLE_VAR( ResourceTracker, string, m_path );
		SERIALIZABLE_VAR( ResourceTracker, string, m_content_hash );
		SERIALIZABLE_VAR( ResourceTracker, uint32_t, m_size );
		SERIALIZABLE_VAR( ResourceTracker, uint64_t, m_modified_epoch );
		SERIALIZABLE_VAR( ResourceTracker, uint64_t, m_modified_ns );

		FILE::MMAP data;
	public:
		ResourceTracker();
		~ResourceTracker();

		void Init(const char* path);
		
		bool operator==(const ResourceTracker& other);
		inline bool operator!=(const ResourceTracker& other)
		{
			return !this->operator==(other);
		}
		friend bfu::stream& operator<<(bfu::stream&, const ResourceTracker& );
	};
	bfu::stream& operator<<(bfu::stream&, const ResourceTracker& );

}	

#endif