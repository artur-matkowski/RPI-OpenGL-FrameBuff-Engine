#ifndef H_ResourceSystem
#define H_ResourceSystem
#include <tuple>
#include <map>
#include "ResourceTrackerManager.hpp"
#include "ResourceSharedReference.hpp"

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
		void SetProjectPath(const char* projectPath);
		inline const char* GetProjectPath() const { return s_projectPath.c_str(); }

		virtual BinaryResourceTracker* RequestBinaryResourceTracker( const UniqueID& ) = 0;
		inline void ScheduleGarbageCollection() { m_needGarbageCollection = true; }
	};










	template<class... ResourceProcessorsTs>
	class ResourceSystem: public ResourceSystemBase
	{
		using ResourceTuple_t = std::tuple< std::map< UniqueID, ResourceReference<ResourceProcessorsTs ...>* > >;
		
		ResourceTuple_t														m_resources;
		ResourceTrackerManager<ResourceProcessorsTs ...>					m_resourceTrackerManager;
		std::tuple< std::pair< ResourceProcessorsTs ..., std::vector< BinaryResourceTracker > > >
																			m_binaryResourceTrackers;


		template<int I>
		void RefreshBinaryResourceTrackers_I()
		{
			//log::warning << "RefreshBinaryResourceTrackers_Is" << std::endl;

			auto& resourceTypeContainer = std::get<I>( m_binaryResourceTrackers );
			const char* typeExt = std::tuple_element_t<I, std::tuple<ResourceProcessorsTs ...>>::GetBinaryOutputFileExtension();
			std::vector<std::string> paths;
			std::string binaryDir = s_projectPath + RESOURCE_BINARIES_DIR;

			resourceTypeContainer.second.clear();
			
			ListFiles( paths, {typeExt}, ListingStrategy::whitelist, binaryDir.c_str() );

			resourceTypeContainer.second.resize( paths.size() );

			for(int i=0; i<paths.size(); i++)
			{
				UniqueID resourcetrackerID;
				UniqueID subResourcetrackerID;
				std::string displayName;

				m_resourceTrackerManager.GetBinaryResourceData( paths[i].c_str(), &resourcetrackerID, &subResourcetrackerID, &displayName );

				resourceTypeContainer.second.emplace_back( resourcetrackerID, subResourcetrackerID, displayName );
			}
		}


		template<int... Is>
		inline void RefreshBinaryResourceTrackers_Is( std::integer_sequence<int, Is...> const & )
		{
			RefreshBinaryResourceTrackers_I<Is...>();
		}


		template<int K>
		void RequestBinaryResourceTracker_K( const UniqueID& id, BinaryResourceTracker* out )
		{
			auto resourceTypeContainer = std::get<K>( m_binaryResourceTrackers );			

			for(auto it = resourceTypeContainer.second.begin(); it!=resourceTypeContainer.second.end(); it++)
			{
				if( it->m_binaryResourceID == id )
				{
					out = &(*it);
					return;
				}
			}
		}

		template<int... Is>
		void RequestBinaryResourceTracker_Is( std::integer_sequence<int, Is...> const &, const UniqueID& id, BinaryResourceTracker* out )
		{
			if( out!=nullptr )
				return;

			RequestBinaryResourceTracker_K<Is...>( id, out );
		}



#ifdef TESTS
	public:
		ResourceTracker* GetResourceTrackerByIndex(int i) { return m_resourceTrackerManager.GetResourceTrackerByIndex(i); }
#endif
	public:
		ResourceSystem(){};
		~ResourceSystem()
		{

		};

		void Init()
		{
			ResourceSystemBase::Init();
		}
		void SetProjectPath(const char* projectPath)
		{
			m_resourceTrackerManager.SetProjectPath( projectPath );
			ResourceSystemBase::SetProjectPath( projectPath );
		}

		template<class SharedResourceReferenceT, class ResourceProcessorT>
		void InitializeResource( UniqueID resourceID, SharedResourceReferenceT* out)
		{
			SharedResourceReferenceT::InitializeObject( resourceID, RequestResourceReference<ResourceProcessorT>, out);
		}

		template<class U>
		static ResourceReference<U>* RequestResourceReference( UniqueID resourceID, ResourceSystemBase* resSys )
		{
			ResourceSystem<ResourceProcessorsTs ...>* _this = (ResourceSystem<ResourceProcessorsTs ...>*)resSys;

			auto& resourceTypeContainer = std::get< std::map< UniqueID, ResourceReference<U>*  > >( _this->m_resources );

			auto it_resourceSearchResoult = resourceTypeContainer.find( resourceID );
			if( it_resourceSearchResoult != resourceTypeContainer.end() ) // resource already loaded
			{
				return it_resourceSearchResoult->second;
			}
			else //resource not used so far
			{
				resourceTypeContainer.emplace( std::make_pair(resourceID, new ResourceReference<U>( resourceID ) ) );

				//auto& resourceTypeContainer = std::get< std::map< UniqueID, ResourceReference<U>*  > >( _this->m_resources );

				auto it = resourceTypeContainer.find( resourceID );
				
				if( it != resourceTypeContainer.end() ) // resource loaded
				{
					return it->second;
				}
			}

			log::warning << "Could not find resource: " << resourceID.ID() << std::endl;
			return nullptr;
		}

		virtual BinaryResourceTracker* RequestBinaryResourceTracker( const UniqueID& id ) override
		{
			BinaryResourceTracker* ret = nullptr;

			constexpr int tupleSize = std::tuple_size<std::tuple<ResourceProcessorsTs ...> >();
			RequestBinaryResourceTracker_Is(std::make_integer_sequence<int, tupleSize>{}, id, ret );

			return ret;
		}

		template<int K>
		void PrintResourceReferencesInUse( bfu::stream& st ) const
		{
			auto resourceTypeContainer = std::get<K>( m_resources );
			const char* resourceExtension = std::tuple_element_t<K, std::tuple<ResourceProcessorsTs ...>>::GetSuportedResourceFileExtension();

			st << "\n\tResource printout for resource type :";
			st << resourceExtension;
			st << ":";
			for(auto it = resourceTypeContainer.begin(); it!=resourceTypeContainer.end(); it++)
			{
				st << "\n\t\t" << it->first.ID();
			}
		}

		template<int K>
		void PrintBinaryResourceTracker( bfu::stream& st ) const
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
				st << "\n\t" << i << std::endl;
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


		template<class... Ts>
		friend bfu::stream& operator<<(bfu::stream&, const ResourceSystem<Ts ...>& );
	};

	template<class... Ts>
	bfu::stream& operator<<(bfu::stream& st, const ResourceSystem<Ts ...>& obj)
	{
		constexpr int tupleSize = std::tuple_size<std::tuple<Ts ...> >();
		obj.Print( std::make_integer_sequence<int, tupleSize>{}, st );
		
		return st;
	}
}

#endif