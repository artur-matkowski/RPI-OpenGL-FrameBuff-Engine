#ifndef H_BinaryResourceTracker
#define H_BinaryResourceTracker
#include "ResourceTrackerManager.hpp"

using bfu::string;


namespace asapi
{

	class BinaryResourceTracker: public bfu::SerializableClassBase<BinaryResourceTracker>
	{
#ifdef TESTS
	public:
#endif		
		SERIALIZABLE_OBJ( ResourceTracker, UniqueID, m_binaryResourceID );
		SERIALIZABLE_OBJ( ResourceTracker, UniqueID, m_resourceTrackerID );
		SERIALIZABLE_VAR( BinaryResourceTracker, string, m_displayedName );

		int ResourceReferenceCounter;
	public:
		BinaryResourceTracker();
		~BinaryResourceTracker()
		{
			if( IsNotReferencedOnScene() )
			{
				DispouseBinaryResourceTracker();
			}
		}

		#ifdef IS_EDITOR
		bool OnGUI( UniqueID* out_newResource, const char* typeFilter )
		{
			//Render resource selector with type restriction
			//return true if resourcewas changes
		}
		#endif
	};

	template<class T>
	class ResourceReference: public IResourceReferenceBase
	{
		#ifdef IS_EDITOR
		//sharedPtr<
		BinaryResourceTracker* binrestracker = nullptr;
		#endif

		void * rendererHandle = nullptr;

		uint16_t 			m_referenceCounter = 0;

		ResourceReference();

		inline void OnLoad2Renderer(UniqueID binaryResourceID)
		{
			#ifdef IS_EDITOR
			if( binaryResourceID==binrestracker.binaryResourceID )
			{
				log::warnign << "Traying to reaply the same resource. That makes no sense, check your code." << std::endl;
				return;
			}
			binrestracker = requestBinaryResourceTracker(binaryResourceID);
			#endif

			T::OnLoad2Renderer( binaryResourceID )
		}

		inline void OnUnload2Renderer()
		{
			T::OnUnload2Renderer( binaryResourceID )
		}

		#ifdef IS_EDITOR
		bool OnGUI( UniqueID* out_newResource, const char* typeFilter)
		{
			return binrestracker->OnGUI( out_newResource, typeFilter );
		}
		#endif

	};




	class ResourceSharedReference: public bfu::SerializableClassBase<ResourceSharedReference>
	{
		char m_resourceType[16];

		SERIALIZABLE_OBJ( ResourceTracker, UniqueID, m_binaryResourceID );

		IResourceReferenceBase* m_resourcePtr = 0;

		ResourceSharedReference(IResourceReferenceBase* resourcePtr) // used by resourceSystem
		{
			//TODO
			resourcePtr->m_referenceCounter++;
			m_resourcePtr = resourcePtr;
		}
	public:
	
		ResourceSharedReference( const ResourceSharedReference& cp )
		{
			//TODO
			m_resourcePtr = cp.m_resourcePtr;
			m_resourcePtr->m_referenceCounter++;
		}

		ResourceSharedReference( ResourceSharedReference&& cp )
		{
			//TODO
			m_resourcePtr = cp.m_resourcePtr;
			cp.m_resourcePtr.SetID(0);
		}
		~ResourceSharedReference()
		{
			m_resourcePtr->m_referenceCounter--;
		}

		#ifdef IS_EDITOR
		void OnGUI()
		{
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
			RenderTYPEpreview();
		};
		#endif


		virtual void PostDeserialize() override
		{
			//if( m_binaryResourceID==0 ) request empty dummy resource for sanity

			rendererResource = requestConsumerResourceReference(m_binaryResourceID);

			if( rendererResource!=nullptr )
				rendererResource.OnUnload2Renderer();
			
			rendererResource.OnLoad2Renderer();
		}
		
	};

}
#endif