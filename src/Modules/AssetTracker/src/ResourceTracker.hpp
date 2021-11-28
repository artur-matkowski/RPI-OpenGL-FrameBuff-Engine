#ifndef H_RESOURCETRACKET
#define H_RESOURCETRACKET
#include "bfu.hpp"
#include "File.hpp"
//resource is a source file like: jpg, glsl, psd, fbx, txt, etc. 

using std::string;

namespace asapi
{
	class ResourceTracker: public bfu::SerializableClassBase<ResourceTracker>
	{
		SERIALIZABLE_VAR( ResourceTracker, uint64_t, m_asset_id );
		SERIALIZABLE_VAR( ResourceTracker, string, m_content_hash );
		SERIALIZABLE_VAR( ResourceTracker, string, m_path );
		SERIALIZABLE_VAR( ResourceTracker, uint32_t, m_size );
		SERIALIZABLE_VAR( ResourceTracker, uint64_t, m_modified_epoch );
		SERIALIZABLE_VAR( ResourceTracker, uint64_t, m_modified_ns );

		FILE::MMAP data;
	public:
		ResourceTracker();
		~ResourceTracker();

		void Init(const char* path);
		
		bool operator==(const ResourceTracker& other);
		friend bfu::stream& operator<<(bfu::stream&, const ResourceTracker& );

	};
	bfu::stream& operator<<(bfu::stream&, const ResourceTracker& );

}	

#endif