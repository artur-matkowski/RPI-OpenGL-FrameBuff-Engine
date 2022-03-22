#ifndef H_ResourceSharedReference
#define H_ResourceSharedReference
#include "bfu.hpp"
#include "ResourceReference.hpp"
#ifdef IS_EDITOR
#include "imgui.h"
#endif
#include "SerializableObject.hpp"
#include "ImGuiSerializer.hpp"
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
	class ResourceSharedReferenceBase: public SerializableObject<T>, public ResourceSharedReferenceInterface
	{
	protected:
		typedef void (*t_callback)(void* data);


		SERIALIZABLE_OBJ( T, UniqueID, 					m_binaryResourceID );
		ResourceReference<ResourceProcessorT>*			m_resourcePtr = nullptr;
		t_callback 										m_callback = nullptr;
		void* 											m_callbackData = nullptr;

	public:
		void BindOnDirtyCallback(t_callback callback, void* callbackData){ m_callback = callback; m_callbackData = callbackData;}

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

		inline void* GetRawHandle() { return m_resourcePtr==nullptr ? nullptr : m_resourcePtr->GetRawHandle(); }

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
				void* handle = m_resourcePtr->GetRawHandle();
				if( handle!=nullptr )
					ResourceProcessorT::OnGUI( m_resourcePtr->GetRawHandle() );
				else
				{
					ImGui::TextColored(
						ImVec4(1.0f, 1.0f, 0.0f, 1.0f)
						, "No valid reference for %s resource"
						, ResourceProcessorT::GetSuportedResourceFileExtension() );
				}
				
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

			if(m_callback!=nullptr)
				m_callback(m_callbackData);
		}
		
	};


}

#endif