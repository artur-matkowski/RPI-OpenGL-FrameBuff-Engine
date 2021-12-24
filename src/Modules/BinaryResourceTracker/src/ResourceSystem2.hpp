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
		std::map< UniqueID, BinaryResourceTracker* > 		m_binaryResourceTrackers;
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
		SharedResourceReferenceT RequestResource( UniqueID resourceID )
		{
			return SharedResourceReferenceT(resourceID, RequestResourceReference<ResourceProcessorT>);
		}

		template<class U>
		static ResourceReference<U>* RequestResourceReference( UniqueID resourceID, ResourceSystemBase* resSys )
		{
			ResourceSystem<ResourceProcessorsTs ...>* _this = (ResourceSystem<ResourceProcessorsTs ...>*)resSys;

			auto& resourceTypeContainer = std::get< std::map< UniqueID, ResourceReference<U> > >( _this->m_resources );

			auto it_resourceSearchResoult = resourceTypeContainer.find( resourceID );
			if( it_resourceSearchResoult != _this->m_resources.end() ) // resource already loaded
			{
				return ResourceSharedReference( *it_resourceSearchResoult );
			}
			else //resource not used so far
			{
				//ResourceReference<T>( resourceID );
				_this->m_resources.emplace( std::make_pair(resourceID, resourceID) );

				auto it = resourceTypeContainer.find( resourceID );
				
				if( it != _this->m_resources.end() ) // resource already loaded
				{
					return ResourceSharedReference( *it );
				}
			}

			log::warning << "Could not find resource: " << resourceID.ID() << std::endl;
			return nullptr;
		}
	};
}

#endif