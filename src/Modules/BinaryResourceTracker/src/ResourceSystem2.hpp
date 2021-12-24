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
		std::map< UniqueID, BinaryResourceTracker*  >
													 		m_binaryResourceTrackers;
		bool 												m_needGarbageCollection = false;

		void RefreshBinaryResourceTrackers();
	public:
		void Init();
		void SetProjectPath(const char* projectPath);
		inline const char* GetProjectPath() const { return s_projectPath.c_str(); }

		BinaryResourceTracker* RequestBinaryResourceTracker( UniqueID );
		inline void ScheduleGarbageCollection() { m_needGarbageCollection = true; }
	};










	template<class... ResourceProcessorsTs>
	class ResourceSystem: public ResourceSystemBase
	{
		std::tuple< std::map< UniqueID, ResourceReference<ResourceProcessorsTs ...>* > >  		
																			m_resources;
		ResourceTrackerManager<ResourceProcessorsTs ...>					m_resourceTrackerManager;


#ifdef TESTS
	public:
		ResourceTracker* GetResourceTrackerByIndex(int i) { return m_resourceTrackerManager.GetResourceTrackerByIndex(i); }
#endif
	public:
		ResourceSystem(){};
		~ResourceSystem(){};

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

		template<int K>
		void PrintI( bfu::stream& st ) const
		{
			auto resourceTypeContainer = std::get<K>( m_resources );

			st << "\n\tResource printout for resource type " << K << ":";
			for(auto it = resourceTypeContainer.begin(); it!=resourceTypeContainer.end(); it++)
			{
				st << "\n\t\t" << it->first.ID();
			}
		}

		void Print( bfu::stream& st ) const
		{
			PrintI<0>( st );
		}

		inline void RefreshResources()
		{
			m_resourceTrackerManager.RefreshResources();
		}


		template<class... Ts>
		friend bfu::stream& operator<<(bfu::stream&, const ResourceSystem<Ts ...>& );
	};

	template<class... Ts>
	bfu::stream& operator<<(bfu::stream& st, const ResourceSystem<Ts ...>& obj)
	{
		obj.Print( st );
		return st;
	}
}

#endif