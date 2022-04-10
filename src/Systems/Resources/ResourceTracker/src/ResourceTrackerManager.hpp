#ifndef H_ResourceTrackerManager
#define H_ResourceTrackerManager
#include <vector>
#include "ResourceTracker.hpp"


namespace asapi
{
	struct SubResourceData
	{
		asapi::UniqueID		m_resourceID;
		std::string 		m_internalID; //subresource identifier to destenquishe subresources in resource file

		SubResourceData(const asapi::UniqueID& resourceID, const std::string& internalID)
			:m_resourceID( resourceID )
			,m_internalID( internalID )
		{}
	};

	class ResourceTrackerManagerBase
	{
	protected:
		std::string						s_assetsDirectoryPath;
		std::string						s_projectDirectoryPath;
		bfu::JSONSerializer				p_JSONSerializer;
		bfu::BinarySerializer			p_BinarySerializer;

		void IterateOverDirtyResourceTrackers();

		void CleanResourceTrackersContainer();
		void DeserializeResourceTrackerContainerFromDisk();

		virtual bool ProcessResource(const asapi::ResourceTracker& in_currentResource
											, asapi::FILE::MMAP* in_resourceFile
											, const char* in_projectPath
											, std::vector<asapi::SubResourceData>* out_resourceBinaries) = 0;

		std::vector<ResourceTracker> 	v_ResourceTrackers;
	public:

		~ResourceTrackerManagerBase();

		void Init(const char* projectPath);
		void SetProjectPath(const char* projectPath);


		//resoruces:
		void RefreshResources();
		void ForceRebuildSubresources();
		ResourceTracker* FindResourceByContentHash(const std::string& content_hash);
		ResourceTracker* FindResourceByFilename(const std::string& filename);
		ResourceTracker* FindResourceByResourceID(const uint64_t& resourceID);
		ResourceTracker* FindResourceByResourceID(const UniqueID& resourceID);


		friend bfu::stream& operator<<(bfu::stream&, const ResourceTrackerManagerBase& );

		inline int CountResouceTrackers() { return v_ResourceTrackers.size(); }
		ResourceTracker* GetResourceTrackerByIndex(int i) { return &(v_ResourceTrackers[i]); }
		int CountSubresources();

		virtual void GetBinaryResourceData(const char* path, UniqueID* out_resourceTrackerID, UniqueID* out_subresourceID, std::string* internalName) = 0;
	};



	template<class... ResourceProcesorTs>
	class ResourceTrackerManager: public ResourceTrackerManagerBase
	{
	protected:
		using ResourceProcesors = std::tuple<ResourceProcesorTs ...>;

		inline bool static IsCompatible(const char* in_fileExtension
											, const asapi::ResourceTracker& in_currentResource)
		{
			return strcmp( in_fileExtension, in_currentResource.GetFileExtension().c_str() ) == 0;
		}

		template<int i>
		inline static void ProcessResourseForType(const asapi::ResourceTracker& in_currentResource
											, asapi::FILE::MMAP* in_resourceFile
											, const char* in_projectPath
											, std::vector<asapi::SubResourceData>* out_resourceBinaries
											, bool *updatedBinaries)
		{
			*updatedBinaries = false;

			const char* resourceExtension = std::tuple_element_t<i, ResourceProcesors>::GetSuportedResourceFileExtension();

			log::debug << "jestem tu" << std::endl;
			
			if(IsCompatible(resourceExtension, in_currentResource))
			{
				*updatedBinaries = std::tuple_element_t<i, ResourceProcesors>::ProcessResource2Binary(in_currentResource
																					, in_resourceFile
																					, in_projectPath
																					, out_resourceBinaries);
			}
		}

		template<int... Is>
		inline static bool ProcesorClassIterator(std::integer_sequence<int, Is...> const &
											, const asapi::ResourceTracker& in_currentResource
											, asapi::FILE::MMAP* in_resourceFile
											, const char* in_projectPath
											, std::vector<asapi::SubResourceData>* out_resourceBinaries)
		{
			using unused = int[];
			bool updatedBinaries = false;
			log::debug << "jestem tu" << std::endl;

			(void)unused { 0, (ProcessResourseForType<Is>(in_currentResource
													, in_resourceFile
													, in_projectPath
													, out_resourceBinaries
													, &updatedBinaries), 0)... };

			return updatedBinaries;
		}


		virtual bool ProcessResource(const asapi::ResourceTracker& in_currentResource
											, asapi::FILE::MMAP* in_resourceFile
											, const char* in_projectPath
											, std::vector<asapi::SubResourceData>* out_resourceBinaries) override
		{
			constexpr int tupleSize = std::tuple_size<ResourceProcesors>();

			log::debug << "jestem tu" << std::endl;

			const bool rebuildedBinarie = ProcesorClassIterator(std::make_integer_sequence<int, tupleSize>{}
																, in_currentResource
																, in_resourceFile
																, in_projectPath
																, out_resourceBinaries );

			return rebuildedBinarie;
		}

	public:
		virtual void GetBinaryResourceData(const char* path, UniqueID* out_resourceTrackerID, UniqueID* out_subresourceID, std::string* out_internalName) override
		{
			for(int i=0; i<v_ResourceTrackers.size(); i++)
			{
				for(int k=0; k<v_ResourceTrackers[i].CountSubresources(); ++k)
				{
					const std::string id_str = std::to_string( v_ResourceTrackers[i].GetSubResourceByIndex(k)->m_resourceID.ID() );
					if( strstr( path, id_str.c_str() )!=0 )
					{
						*out_resourceTrackerID = v_ResourceTrackers[i].GetResourceID();
						*out_subresourceID = v_ResourceTrackers[i].GetSubResourceByIndex(k)->m_resourceID;
						*out_internalName = v_ResourceTrackers[i].GetSubResourceByIndex(k)->m_internalID;

						return;
					}
				}
			}
		}

	};


}

#endif