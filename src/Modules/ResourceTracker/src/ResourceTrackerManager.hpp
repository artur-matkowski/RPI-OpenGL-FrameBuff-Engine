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

		SubResourceData(const std::string& filename, const std::string& internalID)
			:m_filename( filename )
			,m_internalID( internalID )
		{}
	};

	class ResourceTrackerManagerBase
	{
	private:
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

#ifdef TESTS
	public:
		ResourceTracker* GetResourceTrackerByIndex(int i) { return &(v_ResourceTrackers[i]); }
#endif
		std::vector<ResourceTracker> 	v_ResourceTrackers;
	public:

		~ResourceTrackerManagerBase();

		void Init(const char* projectPath);
		void SetProjectPath(const char* projectPath);


		//resoruces:
		void RefreshResources();
		ResourceTracker* FindResourceByContentHash(const std::string& content_hash);
		ResourceTracker* FindResourceByFilename(const std::string& filename);
		ResourceTracker* FindResourceByResourceID(const uint64_t& resourceID);
		ResourceTracker* FindResourceByResourceID(const UniqueID& resourceID);


		friend bfu::stream& operator<<(bfu::stream&, const ResourceTrackerManagerBase& );

		inline int CountResouceTrackers() { return v_ResourceTrackers.size(); }
		int CountSubresources();
	};



	template<class... ResourceProcesorTs>
	class ResourceTrackerManager: public ResourceTrackerManagerBase
	{
	protected:
		using ResourceProcesors = std::tuple<ResourceProcesorTs ...>;

		bool static IsCompatible(const std::vector<std::string>& in_fileExtensions
											, const asapi::ResourceTracker& in_currentResource)
		{
			bool isCompatible = false;
			for(int i=0; i<in_fileExtensions.size(); ++i)
			{
				if( (in_fileExtensions.begin()+i)->compare( in_currentResource.GetFileExtension() )==0 )
				{
					isCompatible = true;
					break;
				}
			}
			return isCompatible;
		}

		template<int i>
		inline static void ProcessResourseForType(const asapi::ResourceTracker& in_currentResource
											, asapi::FILE::MMAP* in_resourceFile
											, const char* in_projectPath
											, std::vector<asapi::SubResourceData>* out_resourceBinaries
											, bool *updatedBinaries)
		{
			std::vector<std::string> fileExtensions;

			*updatedBinaries = false;

			std::tuple_element_t<i, ResourceProcesors>::GetSuportedFilesExtensions( &fileExtensions );

			if(IsCompatible(fileExtensions, in_currentResource))
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

			const bool rebuildedBinarie = ProcesorClassIterator(std::make_integer_sequence<int, tupleSize>{}
																, in_currentResource
																, in_resourceFile
																, in_projectPath
																, out_resourceBinaries );

			return rebuildedBinarie;
		}
	};

}

#endif