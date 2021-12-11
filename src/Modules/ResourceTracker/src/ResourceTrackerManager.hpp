#ifndef H_ResourceTrackerManager
#define H_ResourceTrackerManager
#include <vector>
#include "ResourceTracker.hpp"


namespace asapi
{
	struct SubResourceData
	{
		std::string 		m_filename;
		std::string 		m_internalID; //subresource identifier to destenquishe subresources in resource file
	};

	class ResourceTrackerManager
	{
	public:
		typedef bool (*IterateOverDirtyResourceTrackersCallbackType)(ResourceTracker* in_currentResource
																	, const char* in_projectPath
																	, std::vector<SubResourceData>& out_resourceBinaries);

	private:
		std::string						s_assetsDirectoryPath;
		bfu::JSONSerializer				p_JSONSerializer;
		bfu::BinarySerializer			p_BinarySerializer;

		IterateOverDirtyResourceTrackersCallbackType m_callback = nullptr;
		void IterateOverDirtyResourceTrackers();

#ifdef TESTS
	public:
#endif
		std::vector<ResourceTracker> 	v_ResourceTrackers;
	public:
		void Init(const char* projectPath
			, IterateOverDirtyResourceTrackersCallbackType callback
			);
		void SetProjectPath(const char* projectPath);


		//resoruces:
		void RefreshResources();
		ResourceTracker* FindResourceByContentHash(const std::string& content_hash);
		ResourceTracker* FindResourceByFilename(const std::string& filename);
		ResourceTracker* FindResourceByResourceID(const uint64_t& resourceID);
		ResourceTracker* FindResourceByResourceID(const UniqueID& resourceID);


		friend bfu::stream& operator<<(bfu::stream&, const ResourceTrackerManager& );
	};
}

#endif