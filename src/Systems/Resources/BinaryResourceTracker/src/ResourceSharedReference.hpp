#ifndef H_ResourceSharedReference
#define H_ResourceSharedReference
#include "bfu.hpp"
#include "ResourceReference.hpp"
#ifdef IS_EDITOR
#include "imgui.h"
#endif
#include "ImGUI_Serializer.hpp"
#include "ResourceSystem.hpp"

namespace asapi
{
	//class ResourceSystemBase;

	class ResourceSharedReferenceInterface
	{
	protected:
		static ResourceSystemBase* 							s_resourceSystem;
		static void* 										fs_callback;
	public:
		inline static void SetResourceSystemReference(ResourceSystemBase* resourceSystem) { s_resourceSystem = resourceSystem; }
	};

	template<class T, class ResourceProcessorT>
	class ResourceSharedReferenceBase: public bfu::SerializableClassBase<T>, public ResourceSharedReferenceInterface
	{
	protected:
		SERIALIZABLE_OBJ( T, UniqueID, m_binaryResourceID );


		ResourceReference<ResourceProcessorT>*			m_resourcePtr = nullptr;


	public:


		static void InitializeObject(UniqueID binaryResourceID, T* out)
		{
			out->m_binaryResourceID = std::move( binaryResourceID );
			out->m_resourcePtr = ResourceProcessorT::RequestResourceByProxy( s_resourceSystem, binaryResourceID );
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

		#ifdef IS_EDITOR
		virtual void OnGUI()
		{
			UniqueID newID;
			
			ResourceProcessorT::OnGUI_SelectResourceByProxy( m_binaryResourceID, &newID );

			if( m_binaryResourceID != newID )
			{
				m_binaryResourceID = newID;
				PostDeserializationCallback();
			}
		}
		#endif


		virtual void PostDeserializationCallback() override
		{
			if( m_resourcePtr!=0 )
					m_resourcePtr->DecreaseReferenceCounter();

			m_resourcePtr = ResourceProcessorT::RequestResourceByProxy( s_resourceSystem, m_binaryResourceID );
			m_resourcePtr->IncreaseReferenceCounter();
		}
		
	};


}

#endif