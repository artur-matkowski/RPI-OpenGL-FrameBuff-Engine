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
#ifdef TESTS
	public:
#endif
		SERIALIZABLE_OBJ( ResourceTracker, UniqueID, m_resourceID );
		SERIALIZABLE_VAR( ResourceTracker, string, m_filename );
		SERIALIZABLE_VAR( ResourceTracker, string, m_path );
		SERIALIZABLE_VAR( ResourceTracker, string, m_content_hash );
		SERIALIZABLE_VAR( ResourceTracker, uint32_t, m_size );
		SERIALIZABLE_VAR( ResourceTracker, uint64_t, m_modified_epoch );
		SERIALIZABLE_VAR( ResourceTracker, uint64_t, m_modified_ns ); //modification time in nano seconds since last full second

		FILE::MMAP data;
	public:
		ResourceTracker();
		~ResourceTracker();
		ResourceTracker(const ResourceTracker& cp);
		ResourceTracker(ResourceTracker&& cp) noexcept;


		void Init(const char* path);
		ResourceTracker& operator=(const ResourceTracker& other);
		ResourceTracker& operator=(ResourceTracker&& other);
		
		bool operator==(const ResourceTracker& other);
		bool operator!=(const ResourceTracker& other);

		bool CmpContent(const ResourceTracker& other);
		bool CmpPath(const ResourceTracker& other);

		friend bfu::stream& operator<<(bfu::stream&, const ResourceTracker& );
	};
	bfu::stream& operator<<(bfu::stream&, const ResourceTracker& );

}	

#endif