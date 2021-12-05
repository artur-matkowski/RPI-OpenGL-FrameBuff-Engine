#ifndef H_RESOURCETRACKET
#define H_RESOURCETRACKET
#include "bfu.hpp"
#include "File.hpp"
#include "UniqueID.hpp"
//resource is a source file like: jpg, glsl, psd, fbx, txt, etc. 

using bfu::string;

namespace asapi
{
	class ResourceTrackerManager;

	class ResourceTracker: public bfu::SerializableClassBase<ResourceTracker>
	{
		friend class ResourceTrackerManager;
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

		bool 				isContentDirty = true; 	//Need processing
		bool 				m_outDated = false; //true if resource tracker json file is to be deleted upon destruction

		static std::string 	_ResourceTrackersPath;

	public:
		ResourceTracker();
		~ResourceTracker();
		ResourceTracker(const ResourceTracker& cp);
		ResourceTracker(ResourceTracker&& cp) noexcept;


		static void SetProjectPath(const char* path);
		#ifdef IS_EDITOR
		static std::string GetContentHash(const char* path, uint32_t* out_size );
		#endif


		void Init(const char* path);
		ResourceTracker& operator=(const ResourceTracker& other);
		ResourceTracker& operator=(ResourceTracker&& other);
		
		bool operator==(const ResourceTracker& other);
		bool operator!=(const ResourceTracker& other);

		bool CmpContent(const ResourceTracker& other);
		bool CmpPath(const ResourceTracker& other);

		friend bfu::stream& operator<<(bfu::stream&, const ResourceTracker& );

		inline void SetContentDirty(bool isContentDirty) { this->isContentDirty = isContentDirty; }
		inline bool IsContentDirty() { return isContentDirty; }
		inline void MarkOutdated() { m_outDated = true; }
		inline uint64_t GetResourceID(){ return m_resourceID.ID(); }
		inline std::string GetFilename(){ return m_filename; }
		std::string GetFileExtension();


		void ObtainResourceOwnership(ResourceTracker & source);

		std::string GetResourceTrackerPath();

	private:
		void RemoveResourceTrackerFile();
	};
	bfu::stream& operator<<(bfu::stream&, const ResourceTracker& );

}	

#endif