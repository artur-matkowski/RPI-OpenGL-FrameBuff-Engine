#ifndef H_ResourceSystem
#define H_ResourceSystem
#include <tuple>
#include <map>
#include "ResourceTrackerManager.hpp"
#include "ResourceSharedReference.hpp"
#include "ResourceProcessorBase.hpp"
#include "imgui.h"

namespace asapi
{


	class ResourceSystemBase
	{
	protected:
		std::string 										s_projectPath;
		bool 												m_needGarbageCollection = false;

	public:
		~ResourceSystemBase(){};

		void Init();
		inline const char* GetProjectPath() const { return s_projectPath.c_str(); }

		virtual BinaryResourceTracker* RequestBinaryResourceTracker( const UniqueID& ) = 0;
		inline void ScheduleGarbageCollection() { m_needGarbageCollection = true; }		
	};










	template<class... ResourceProcessorsTs>
	class ResourceSystem: public ResourceSystemBase
	{
		using ResourceTuple_t = std::tuple< std::map< UniqueID, ResourceReference<ResourceProcessorsTs>* > ...>;
		
		static ResourceSystem<ResourceProcessorsTs ...>*					s_this;
		ResourceTuple_t														m_resources;
		ResourceTrackerManager<ResourceProcessorsTs ...>					m_resourceTrackerManager;
		std::tuple< std::pair< ResourceProcessorsTs, std::vector< BinaryResourceTracker > > ...>
																			m_binaryResourceTrackers;


		template<int I>
		inline void 		InitializeResourceProcessorT_I();

		template<int... Is>
		inline void 		InitializeResourceProcessorT_Is( std::integer_sequence<int, Is...> const & );




		template<int I>
		inline void 		RefreshBinaryResourceTrackers_I();

		template<int... Is>
		inline void 		RefreshBinaryResourceTrackers_Is( std::integer_sequence<int, Is...> const & );




		template<int K>
		inline void			RequestBinaryResourceTracker_K( const UniqueID& id, BinaryResourceTracker* out );

		template<int... Is>
		inline void 		RequestBinaryResourceTracker_Is( std::integer_sequence<int, Is...> const &, const UniqueID& id, BinaryResourceTracker* out );




		template<int I>
		inline void 		Update_I();

		template<int... Is>
		inline void 		Update_Is( std::integer_sequence<int, Is...> const & );


	public:
							ResourceSystem()						{};
							~ResourceSystem()						{};

		
		ResourceTracker* 	GetResourceTrackerByIndex(int i) 		{ return m_resourceTrackerManager.GetResourceTrackerByIndex(i); }

		void 				Init();
		void 				SetProjectPath(const char* projectPath);
		void 				Update();
		void 				ForceRebuildSubresources()				{ m_resourceTrackerManager.ForceRebuildSubresources(); }




		template
			< class SharedResourceReferenceT
			, class ResourceProcessorT >
		void 				InitializeResource( UniqueID resourceID
											, SharedResourceReferenceT* out);


		template
			<class ResourceProcessorT>
		ResourceReference<ResourceProcessorT>* RequestResourceReference( UniqueID resourceID );


		template
			<class ResourceProcessorT>
		static ResourceReference<ResourceProcessorT>* sRequestResourceReference( ResourceSystemBase* system, UniqueID resourceID );


		virtual BinaryResourceTracker* RequestBinaryResourceTracker( const UniqueID& id ) override;

		template<int K>
		void 						PrintResourceReferencesInUse( bfu::stream& st ) const;

		template<int K>
		void 						PrintBinaryResourceTracker( bfu::stream& st ) const
		{
			auto resourceTypeContainer = std::get<K>( m_binaryResourceTrackers );
			const char* resourceExtension = std::tuple_element_t<K, std::tuple<ResourceProcessorsTs ...>>::GetSuportedResourceFileExtension();

			st << "\n\tBinary resource tracker printout for resource type :";
			st << resourceExtension;
			st << ":";
			int i=0;
			for(auto it = resourceTypeContainer.second.begin(); it!=resourceTypeContainer.second.end(); it++)
			{
				i++;
				st << "\n\t" << i;
				st << *it;
			}
		}

		template<int... Is>
		void Print( std::integer_sequence<int, Is...> const &, bfu::stream& st ) const
		{
			PrintResourceReferencesInUse<Is...>( st );
			PrintBinaryResourceTracker<Is...>( st );
		}

		inline void RefreshResources()
		{
			m_resourceTrackerManager.RefreshResources();

			constexpr int tupleSize = std::tuple_size<std::tuple<ResourceProcessorsTs ...> >();
			RefreshBinaryResourceTrackers_Is( std::make_integer_sequence<int, tupleSize>{} );
		}

		#ifdef IS_EDITOR
		template<int I>
		inline void 		OnGUI_I();

		template<int... Is>
		inline void 		OnGUI_Is( std::integer_sequence<int, Is...> const & );

		void OnGUI();
		#endif


		template<class... Ts>
		friend bfu::stream& operator<<(bfu::stream&, const ResourceSystem<Ts ...>& );


		#ifdef IS_EDITOR
		template<class ResourceProcessorT>
		static void OnGUI_SelectResource( const UniqueID& in_resourceID, UniqueID* out_newResourceID );

		template<class ResourceProcessorT>
		static bool ResourceComboGetter(void* data, int idx, const char** out_text);
		#endif
	};


















	template<class... ResourceProcessorsTs>
	ResourceSystem<ResourceProcessorsTs ...>* ResourceSystem<ResourceProcessorsTs ...>::s_this = nullptr;


	template<class... ResourceProcessorsTs>
	void ResourceSystem<ResourceProcessorsTs ...>::Init() 
	{ 
		s_this = this;
		ResourceSystemBase::Init(); 


		constexpr int tupleSize = std::tuple_size<std::tuple<ResourceProcessorsTs ...> >();
		InitializeResourceProcessorT_Is( std::make_integer_sequence<int, tupleSize>{} );
	}

	template<class... ResourceProcessorsTs>
	template<int I>
	void ResourceSystem<ResourceProcessorsTs ...>::Update_I()
	{
		auto& resourceTypeContainer = std::get<I>( m_resources );

		for(auto it = resourceTypeContainer.begin(); it!=resourceTypeContainer.end(); )
		{
			if( it->second->GetReferenceCounter() <= 0)
			{
				delete it->second;
				it = resourceTypeContainer.erase( it );
			}
			else
			{
				it++;
			}
		}
	}


	template<class... ResourceProcessorsTs>
	template<int... Is>
	void ResourceSystem<ResourceProcessorsTs ...>::Update_Is( std::integer_sequence<int, Is...> const & )
	{
		using unused = int[];
		(void)unused { 0, (Update_I<Is>(), 0)... };
	}

	template<class... ResourceProcessorsTs>
	void ResourceSystem<ResourceProcessorsTs ...>::Update() 
	{ 
		if( m_needGarbageCollection )
		{
			constexpr int tupleSize = std::tuple_size<std::tuple<ResourceProcessorsTs ...> >();
			Update_Is( std::make_integer_sequence<int, tupleSize>{} );

			m_needGarbageCollection = false;
		}
	}


	template<class... ResourceProcessorsTs>
	template<int I>
	void ResourceSystem<ResourceProcessorsTs ...>::InitializeResourceProcessorT_I()
	{
		auto& resourceTypeContainer = std::get<I>( m_binaryResourceTrackers );
		using ResourceProcessorT = std::tuple_element_t<I, std::tuple<ResourceProcessorsTs ...>>;

		using RequestCallbackT = typename ResourceProcessorBase<ResourceProcessorT>::RequestCallbackT;
		RequestCallbackT requestCallback = (RequestCallbackT) &ResourceSystem<ResourceProcessorsTs ...>::sRequestResourceReference<ResourceProcessorT>;
		
		using SelectCallbackT = typename ResourceProcessorBase<ResourceProcessorT>::OnGUI_SelectResourceCallbackT;
		SelectCallbackT selectCallback = (SelectCallbackT) &ResourceSystem<ResourceProcessorsTs ...>::OnGUI_SelectResource<ResourceProcessorT>;
		

		ResourceProcessorT::Initialize( requestCallback, selectCallback );
	}

	template<class... ResourceProcessorsTs>
	template<int... Is>
	void ResourceSystem<ResourceProcessorsTs ...>::InitializeResourceProcessorT_Is( std::integer_sequence<int, Is...> const & )
	{
		using unused = int[];
		(void)unused { 0, (InitializeResourceProcessorT_I<Is>(), 0)... };
	}


	template<class... ResourceProcessorsTs>
	template<int I>
	void ResourceSystem<ResourceProcessorsTs ...>::RefreshBinaryResourceTrackers_I()
	{
		auto& resourceTypeContainer = std::get<I>( m_binaryResourceTrackers );
		const char* typeExt = std::tuple_element_t<I, std::tuple<ResourceProcessorsTs ...>>::GetBinaryOutputFileExtension();
		char typeExtBuff[64];
		std::vector<std::string> paths;
		std::string binaryDir = s_projectPath + RESOURCE_BINARIES_DIR;

		snprintf(typeExtBuff, 64, "%s.bin", typeExt);
		resourceTypeContainer.second.clear();
		
		ListFiles( paths, {typeExtBuff}, ListingStrategy::whitelist, binaryDir.c_str() );

		resourceTypeContainer.second.reserve( paths.size() );


		for(int i=0; i<paths.size(); i++)
		{
			UniqueID resourcetrackerID( (uint64_t)0 );
			UniqueID subResourcetrackerID( (uint64_t)0 );
			std::string displayName;

			m_resourceTrackerManager.GetBinaryResourceData( paths[i].c_str()
														, &resourcetrackerID
														, &subResourcetrackerID
														, &displayName );

			resourceTypeContainer.second.emplace_back( resourcetrackerID
														, subResourcetrackerID
														, displayName
														, typeExt );
		}
	}


	template<class... ResourceProcessorsTs>
	template<int... Is>
	void ResourceSystem<ResourceProcessorsTs ...>::RefreshBinaryResourceTrackers_Is( std::integer_sequence<int, Is...> const & )
	{
		using unused = int[];
		(void)unused { 0, (RefreshBinaryResourceTrackers_I<Is>(), 0)... };
	}


	template<class... ResourceProcessorsTs>
	template<int K>
	void ResourceSystem<ResourceProcessorsTs ...>::RequestBinaryResourceTracker_K( const UniqueID& id, BinaryResourceTracker* out )
	{
		auto resourceTypeContainer = std::get<K>( m_binaryResourceTrackers );			

		for(auto it = resourceTypeContainer.second.begin(); it!=resourceTypeContainer.second.end(); it++)
		{
			if( it->GetSubResourceID() == id )
			{
				out = &(*it);
				return;
			}
		}
	}


	template<class... ResourceProcessorsTs>
	template<int... Is>
	void ResourceSystem<ResourceProcessorsTs ...>::RequestBinaryResourceTracker_Is( std::integer_sequence<int, Is...> const &, const UniqueID& id, BinaryResourceTracker* out )
	{
		if( out!=nullptr )
			return;

		using unused = int[];
		(void)unused { 0, (RequestBinaryResourceTracker_K<Is>( id, out ), 0)... };
	}

	template<class... ResourceProcessorsTs>
	void ResourceSystem<ResourceProcessorsTs ...>::SetProjectPath(const char* projectPath)
	{
		m_resourceTrackerManager.SetProjectPath( projectPath );
		BinaryResourceTracker::SetProjectPath( projectPath );
		IResourceReferenceBase::SetProjectPath( projectPath );		
		s_projectPath = projectPath;

		RefreshResources();
	}

	template<class... ResourceProcessorsTs>
	template<class SharedResourceReferenceT, class ResourceProcessorT>
	void ResourceSystem<ResourceProcessorsTs ...>::InitializeResource( UniqueID resourceID, SharedResourceReferenceT* out)
	{
		SharedResourceReferenceT::InitializeObject( resourceID, out);
	}


	template<class... ResourceProcessorsTs>
	template<class U>
	ResourceReference<U>* ResourceSystem<ResourceProcessorsTs ...>::RequestResourceReference( UniqueID resourceID )
	{
		auto& resourceTypeContainer = std::get< std::map< UniqueID, ResourceReference<U>* > >( this->m_resources );

		auto it_resourceSearchResoult = resourceTypeContainer.find( resourceID );
		if( it_resourceSearchResoult != resourceTypeContainer.end() ) // resource already loaded
		{
			return it_resourceSearchResoult->second;
		}
		else //resource not used so far
		{
			resourceTypeContainer.emplace( std::make_pair(resourceID, new ResourceReference<U>( resourceID ) ) );

			auto it = resourceTypeContainer.find( resourceID );
			
			if( it != resourceTypeContainer.end() ) // resource loaded
			{
				return it->second;
			}
		}

		log::warning << "Could not find resource: " << resourceID.ID() << std::endl;
		return nullptr;
	}

	template<class... ResourceProcessorsTs>
	template<class U>
	ResourceReference<U>* ResourceSystem<ResourceProcessorsTs ...>::sRequestResourceReference( ResourceSystemBase* system, UniqueID resourceID )
	{
		ResourceSystem<ResourceProcessorsTs ...>* _this = (ResourceSystem<ResourceProcessorsTs ...>*)system;

		return _this->RequestResourceReference<U>(resourceID);
	}

	template<class... ResourceProcessorsTs>
	BinaryResourceTracker* ResourceSystem<ResourceProcessorsTs ...>::RequestBinaryResourceTracker( const UniqueID& id )
	{
		BinaryResourceTracker* ret = nullptr;

		constexpr int tupleSize = std::tuple_size<std::tuple<ResourceProcessorsTs ...> >();
		RequestBinaryResourceTracker_Is(std::make_integer_sequence<int, tupleSize>{}, id, ret );

		return ret;
	}


	template<class... ResourceProcessorsTs>
	template<int K>
	void ResourceSystem<ResourceProcessorsTs ...>::PrintResourceReferencesInUse( bfu::stream& st ) const
	{
		auto& resourceTypeContainer = std::get<K>( m_resources );
		const char* resourceExtension = std::tuple_element_t<K, std::tuple<ResourceProcessorsTs ...>>::GetSuportedResourceFileExtension();

		st << "\n\tResource printout for resource type :";
		st << resourceExtension;
		st << ":";
		for(auto it = resourceTypeContainer.begin(); it!=resourceTypeContainer.end(); it++)
		{
			st << "\n\t\t" << it->first.ID();
		}
	}


	#ifdef IS_EDITOR

	template<class... ResourceProcessorsTs>
	template<int I>
	void ResourceSystem<ResourceProcessorsTs ...>::OnGUI_I()
	{
		auto& resourceTypeContainer = std::get<I>( m_binaryResourceTrackers );
		const char* typeExt = std::tuple_element_t<I, std::tuple<ResourceProcessorsTs ...>>::GetBinaryOutputFileExtension();
		char resourceType[128];
		static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

		sprintf(resourceType, "Resources found for resource type: %s", typeExt);

		if (ImGui::CollapsingHeader(resourceType))
		{
			if (ImGui::BeginTable("Resources:", 4, flags))
			{
				ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_NoResize);
				ImGui::TableSetupColumn("Input Resource ID", ImGuiTableColumnFlags_NoResize);
				ImGui::TableSetupColumn("Binary Resource ID", ImGuiTableColumnFlags_NoResize);
				ImGui::TableSetupColumn("Resource Display Name", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableHeadersRow();


				int i=0;
				for(auto it = resourceTypeContainer.second.begin(); it!=resourceTypeContainer.second.end(); it++)
				{
					ImGui::TableNextRow();

                    ImGui::TableSetColumnIndex(0);
					ImGui::Text("%d", i);
					
                    ImGui::TableSetColumnIndex(1);
					ImGui::Text("%llu", it->GetResourceID().ID() );
					
                    ImGui::TableSetColumnIndex(2);
					ImGui::Text("%llu", it->GetSubResourceID().ID() );

                    ImGui::TableSetColumnIndex(3);
                    char buff[128];
                    char tagbuff[64];
                    snprintf( tagbuff, 64, "<-Resource Name##%d", i);
                    strncpy(buff, it->GetDisplayName(), 128);
					if( ImGui::InputText(tagbuff, buff, 128) )
					{
						it->SetDisplayName( buff );
					}

					++i;
				}

				ImGui::EndTable();
			}
		}
	}

	template<class... ResourceProcessorsTs>
	template<int... Is>
	void ResourceSystem<ResourceProcessorsTs ...>::OnGUI_Is( std::integer_sequence<int, Is...> const & )
	{
		using unused = int[];
		(void)unused { 0, (OnGUI_I<Is>(), 0)... };
	}

	template<class... ResourceProcessorsTs>
	void ResourceSystem<ResourceProcessorsTs ...>::OnGUI()
	{
		constexpr int tupleSize = std::tuple_size<std::tuple<ResourceProcessorsTs ...> >();
		OnGUI_Is( std::make_integer_sequence<int, tupleSize>{} );
	}
	#endif



	template<class... Ts>
	bfu::stream& operator<<(bfu::stream& st, const ResourceSystem<Ts ...>& obj)
	{
		constexpr int tupleSize = std::tuple_size<std::tuple<Ts ...> >();
		obj.Print( std::make_integer_sequence<int, tupleSize>{}, st );
		
		return st;
	}



	#ifdef IS_EDITOR
	template<class... ResourceProcesorTs>
	template<class ResourceProcessorT>
	void ResourceSystem<ResourceProcesorTs ...>::OnGUI_SelectResource( const UniqueID& in_resourceID, UniqueID* out_newResourceID )
	{
		ResourceSystem<ResourceProcesorTs ...>* _this = s_this;
		*out_newResourceID = in_resourceID;

		std::vector< BinaryResourceTracker >& resourceContainer = std::get< std::pair< ResourceProcessorT, std::vector< BinaryResourceTracker > > >( _this->m_binaryResourceTrackers ).second;
		int currentIndex = -1;

		for(int i=0; i<resourceContainer.size(); i++)
		{
			if( in_resourceID==resourceContainer[i].GetSubResourceID() )
			{
				currentIndex = i;
				break;
			}
		}
		char text[512];

		snprintf(text, 512, "Select \"%s\" resource", ResourceProcessorT::GetBinaryOutputFileExtension() );

		if( ImGui::Combo( text
						, &currentIndex
						, &ResourceComboGetter<ResourceProcessorT>
						, _this
						, resourceContainer.size() ) )
		{
			*out_newResourceID = resourceContainer[currentIndex].GetSubResourceID();			
		}
	}

	template<class... ResourceProcesorTs>
	template<class ResourceProcessorT>
	bool ResourceSystem<ResourceProcesorTs ...>::ResourceComboGetter(void* data, int idx, const char** out_text)
	{
		ResourceSystem<ResourceProcesorTs ...>* _this = (ResourceSystem<ResourceProcesorTs ...>*)data;

		std::vector< BinaryResourceTracker >& resourceContainer = std::get< std::pair< ResourceProcessorT, std::vector< BinaryResourceTracker > > >( _this->m_binaryResourceTrackers ).second;
		
		*out_text = resourceContainer[idx].GetDisplayName();

		return true;
	}
	#endif
}

#endif