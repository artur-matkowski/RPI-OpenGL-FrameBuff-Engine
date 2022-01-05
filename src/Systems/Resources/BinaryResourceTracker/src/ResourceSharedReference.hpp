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


		ResourceSharedReferenceBase()
		{
			m_resourcePtr = nullptr;
			m_binaryResourceID = 0;
		};
	
		ResourceSharedReferenceBase( const ResourceSharedReferenceBase& cp )
		{
			m_resourcePtr->DecreaseReferenceCounter();
			m_resourcePtr = cp.m_resourcePtr;
			m_binaryResourceID = cp.m_binaryResourceID;
			m_resourcePtr->IncreaseReferenceCounter();
		}

		ResourceSharedReferenceBase( ResourceSharedReferenceBase&& cp )
		{
			m_resourcePtr = cp.m_resourcePtr;
			m_binaryResourceID = std::move( cp.m_binaryResourceID );
			cp.m_resourcePtr = nullptr;
		}
		~ResourceSharedReferenceBase()
		{
			if( m_resourcePtr!=0 )
				m_resourcePtr->DecreaseReferenceCounter();
		}

		#ifdef IS_EDITOR
		virtual void OnGUI(){}
		void OnGUI_caller()
		{
			UniqueID newID;
			
			ResourceProcessorT::OnGUI_SelectResourceByProxy( m_binaryResourceID, &newID );

			if( m_binaryResourceID != newID )
			{
				m_binaryResourceID = newID;
				PostDeserializationCallback();
			}

			if( m_resourcePtr!=nullptr )
			{
				ResourceProcessorT::OnGUI( m_resourcePtr->GetRawHandle() );
				this->OnGUI();
				m_resourcePtr->OnGUI();
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