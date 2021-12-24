#ifndef H_ResourceSharedReference
#define H_ResourceSharedReference
#include "bfu.hpp"
#include "ResourceReference.hpp"

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
		SERIALIZABLE_OBJ( ResourceSharedReferenceBase, UniqueID, m_binaryResourceID );

		typedef ResourceReference<ResourceProcessorT>* (*RequestCallbackT)( UniqueID, ResourceSystemBase* );


		ResourceReference<ResourceProcessorT>*			m_resourcePtr = nullptr;
		RequestCallbackT 								m_callback;


		ResourceSharedReferenceBase(IResourceReferenceBase* resourcePtr) // used by resourceSystem
		{
			//TODO
			m_resourcePtr = resourcePtr;
			m_resourcePtr->IncreaseReferenceCounter();
		}

		ResourceSharedReferenceBase(UniqueID binaryResourceID, RequestCallbackT callback) // used by resourceSystem
		{
			//TODO
			m_binaryResourceID = binaryResourceID;
			m_callback = callback;
			m_resourcePtr = m_callback(m_binaryResourceID, s_resourceSystem);
			m_resourcePtr->IncreaseReferenceCounter();
		}
	public:


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

		#ifdef IS_EDITOR
		void OnGUI()
		{
			/*
			ImGUI.Label("Resource type: %s", m_resourceType);
			
			UniqueID resourceID;
			//TODO
			if( rendererResource.OnGUI( &resourceID, m_resourceType ) ) //changed resourceID
			{
				m_binaryResourceID = resourceID;
				rendererResource = requestConsumerResourceReference(resourceID);
				rendererResource.OnUnload2Renderer();
				rendererResource.OnLoad2Renderer();
			}

			//optional
			RenderTYPEpreview();*/
		};
		#endif


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