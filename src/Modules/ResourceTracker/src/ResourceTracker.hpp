#ifndef H_RESOURCETRACKET
#define H_RESOURCETRACKET
#include "bfu.hpp"
#include "File.hpp"
#include "UniqueID.hpp"
//resource is a source file like: jpg, glsl, psd, fbx, txt, etc. 

#define RESOURCE_TRACKERS_DIR  "/assets/Resource_Trackers"
#define RESOURCE_BINARIES_DIR "/assets/Resource_Binaries"
#define ASSETS_DIR "/assets"

using bfu::string;



namespace asapi
{
	//class ResourceTrackerManager;
	class SubResourceData;

	class SerializableSubResourceData: public bfu::SerializableClassBase<SerializableSubResourceData>
	{
	public:
		SERIALIZABLE_VAR( SerializableSubResourceData, string, m_filename );
		SERIALIZABLE_VAR( SerializableSubResourceData, string, m_internalID ); //subresource identifier to destenquishe subresources in resource file


		SerializableSubResourceData();
		SerializableSubResourceData( const SubResourceData& in_data );
		SerializableSubResourceData( SerializableSubResourceData&& in_data );
		SerializableSubResourceData& operator=( SubResourceData&& in_data );

		friend bfu::stream& operator<<(bfu::stream&, const SerializableSubResourceData& );
	};

	class ResourceTracker: public bfu::SerializableClassBase<ResourceTracker>
	{
		//friend class ResourceTrackerManager;
#ifdef TESTS
	public:
#endif
		SERIALIZABLE_OBJ( ResourceTracker, UniqueID, m_resourceID );
		SERIALIZABLE_VAR( ResourceTracker, string, m_filename );
		SERIALIZABLE_VAR( ResourceTracker, string, m_resTrackerFilename );
		SERIALIZABLE_VAR( ResourceTracker, string, m_path );
		SERIALIZABLE_VAR( ResourceTracker, string, m_content_hash );
		SERIALIZABLE_VAR( ResourceTracker, uint32_t, m_size );
		SERIALIZABLE_VAR( ResourceTracker, uint64_t, m_modified_epoch );
		SERIALIZABLE_VAR( ResourceTracker, uint64_t, m_modified_ns ); //modification time in nano seconds since last full second
		SERIALIZABLE_OBJ_VEC( ResourceTracker, SerializableSubResourceData, v_subresources );

		bool 				isContentDirty = true; 	//Need processing
		

		static std::string 	_ResourceTrackersPath;
		static std::string 	_ProjectPath;

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
		
		bool operator==(const ResourceTracker& other) const;
		bool operator!=(const ResourceTracker& other) const;

		bool CmpContent(const ResourceTracker& other) const;
		bool CmpPath(const ResourceTracker& other) const;

		friend bfu::stream& operator<<(bfu::stream&, const ResourceTracker& );

		inline void SetContentDirty(bool isContentDirty) { this->isContentDirty = isContentDirty; }
		inline bool IsContentDirty() const { return isContentDirty; }




		inline uint64_t GetResourceID() const { return m_resourceID.ID(); }
		inline std::string GetFilename() const { return m_filename; }
		inline std::string GetPath() const { return m_path; }
		inline std::string GetFullPath() const { return _ProjectPath+std::string(ASSETS_DIR)+std::string("/")+m_path.c_str(); }
		std::string GetFileExtension() const;
		inline std::string GetContentHash() const { return m_content_hash; }


		void MoveSubresources(std::vector<SubResourceData>* in_subresources);


		void ObtainResourceOwnership(ResourceTracker & source);
		inline bool IsResourceOwner() const { return m_resourceID.ID() != 0; }
		inline void DetachFromResourceOnApplicationCLose(){ m_resourceID.SetID(0); }

		std::string GetResourceTrackerPath() const;

		inline int CountSubresources() const { return v_subresources.size(); }
		bool FindSubResourceByInternalID( const std::string& in_id, std::string& out_filename ) const;

	private:
		void RemoveResourceTrackerFile();
		void RemoveResourceBinaryFile();
	};
	bfu::stream& operator<<(bfu::stream&, const ResourceTracker& );
	bfu::stream& operator<<(bfu::stream&, const SerializableSubResourceData& );

}	

#endif