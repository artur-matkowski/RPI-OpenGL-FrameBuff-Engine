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

	protected:
		static RequestCallbackT 						m_requestCallbackCallback;

	public:
		static void Initialize( RequestCallbackT callback ){ m_requestCallbackCallback = callback; }

		inline static ResourceReference<ResourceProcessorT>* RequestResourceByProxy( ResourceSystemBase* res, UniqueID id )
		{
			return m_requestCallbackCallback(res, id);
		}

	};


	template<class ResourceProcessorT>
	typename ResourceProcessorBase<ResourceProcessorT>::RequestCallbackT ResourceProcessorBase<ResourceProcessorT>::m_requestCallbackCallback = nullptr;
}

#endif