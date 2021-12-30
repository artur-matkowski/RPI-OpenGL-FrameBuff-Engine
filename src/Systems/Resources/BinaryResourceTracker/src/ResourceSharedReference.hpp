#ifndef H_ResourceSharedReference
#define H_ResourceSharedReference
#include "bfu.hpp"
#include "ResourceReference.hpp"
#ifdef IS_EDITOR
#include "imgui.h"
#endif
#include "ImGUI_Serializer.hpp"

namespace asapi
{
	class ResourceSystemBase;

	class ResourceSharedReferenceInterface
	{
	protected:
		static ResourceSystemBase* 							s_resourceSystem;
	public:
		inline static void SetResourceSystemReference(ResourceSystemBase* resourceSystem) { s_resourceSystem = resourceSystem; }
	};

	template<class T, class ResourceProcessorT>
	class ResourceSharedReferenceBase: public bfu::SerializableClassBase<T>, public ResourceSharedReferenceInterface
	{
	protected:
		SERIALIZABLE_OBJ( T, UniqueID, m_binaryResourceID );




		typedef ResourceReference<ResourceProcessorT>* (*RequestCallbackT)( UniqueID, ResourceSystemBase* );


		ResourceReference<ResourceProcessorT>*			m_resourcePtr = nullptr;
		RequestCallbackT 								m_callback;


	public:
		static void InitializeObject(UniqueID binaryResourceID, RequestCallbackT callback, T* out)
		{
			out->m_binaryResourceID = std::move( binaryResourceID );
			out->m_callback = callback;
			out->m_resourcePtr = out->m_callback(out->m_binaryResourceID, s_resourceSystem);
			out->m_resourcePtr->IncreaseReferenceCounter();
		}


		ResourceSharedReferenceBase(){};
	
		ResourceSharedReferenceBase( const ResourceSharedReferenceBase& cp )
		{
			//TODO
			m_resourcePtr = cp.m_resourcePtr;
			m_resourcePtr->IncreaseReferenceCounter();
		}

		ResourceSharedReferenceBase( ResourceSharedReferenceBase&& cp )
		{
			//TODO
			m_resourcePtr = cp.m_resourcePtr;
			cp.m_resourcePtr = nullptr;
		}
		~ResourceSharedReferenceBase()
		{
			if( m_resourcePtr!=0 )
				m_resourcePtr->DecreaseReferenceCounter();
		}


		virtual void PostDeserializationCallback() override
		{
			if( m_resourcePtr!=0 )
					m_resourcePtr->DecreaseReferenceCounter();

			m_resourcePtr = m_callback(m_binaryResourceID, s_resourceSystem);
			m_resourcePtr->IncreaseReferenceCounter();
		}
		
	};
}

#endif