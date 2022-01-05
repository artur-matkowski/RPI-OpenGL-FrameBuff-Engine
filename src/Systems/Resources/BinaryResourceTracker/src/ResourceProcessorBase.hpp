#ifndef H_ResourceProcessorBase
#define H_ResourceProcessorBase
#include "ResourceSystem.hpp"

namespace asapi
{

	template<class ResourceProcessorT>
	class ResourceProcessorBase
	{
	public:
		typedef ResourceReference<ResourceProcessorT>* (*RequestCallbackT)( ResourceSystemBase*, UniqueID );
		typedef void (*OnGUI_SelectResourceCallbackT)( const UniqueID& in_resourceID, UniqueID* out_newResourceID );

	protected:
		static RequestCallbackT 						m_requestCallbackCallback;
		static OnGUI_SelectResourceCallbackT			m_OnGUI_SelectResourceCallback;

	public:
		static void Initialize( RequestCallbackT requestCallback, OnGUI_SelectResourceCallbackT selectCallback )
		{ 
			m_requestCallbackCallback = requestCallback; 
			m_OnGUI_SelectResourceCallback = selectCallback;
		}

		inline static ResourceReference<ResourceProcessorT>* RequestResourceByProxy( ResourceSystemBase* res, UniqueID id )
		{
			return m_requestCallbackCallback(res, id);
		}

		inline static void OnGUI_SelectResourceByProxy( const UniqueID& in_resourceID, UniqueID* out_newResourceID )
		{
			m_OnGUI_SelectResourceCallback(in_resourceID, out_newResourceID);
		}

	};


	template<class ResourceProcessorT>
	typename ResourceProcessorBase<ResourceProcessorT>::RequestCallbackT ResourceProcessorBase<ResourceProcessorT>::m_requestCallbackCallback = nullptr;

	template<class ResourceProcessorT>
	typename ResourceProcessorBase<ResourceProcessorT>::OnGUI_SelectResourceCallbackT ResourceProcessorBase<ResourceProcessorT>::m_OnGUI_SelectResourceCallback = nullptr;
}

#endif