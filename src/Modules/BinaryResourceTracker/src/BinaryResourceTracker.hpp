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

	class ConsumerResourceReferenceTXT: public IConsumerResourceReferenceBase
	{
		#ifdef IS_EDITOR
		//sharedPtr<
		BinaryResourceTracker binrestracker;
		#endif

		void * rendererHandle = nullptr;

		ConsumerResourceReferenceTXT();

		void OnLoad2Renderer(UniqueID m_binaryResourceID)
		{
			#ifdef IS_EDITOR
			if( m_binaryResourceID==binrestracker.m_binaryResourceID )
			{
				log::warnign << "Traying to reaply the same resource. That makes no sense, check your code." << std::endl;
				return;
			}
			binrestracker = requestBinaryResourceTracker(m_binaryResourceID);
			#endif

			//TODO
		}

		void OnUnload2Renderer()
		{
			//TODO
		}

		#ifdef IS_EDITOR
		bool OnGUI( UniqueID* out_newResource, const char* typeFilter)
		{
			return binrestracker->OnGUI( out_newResource, typeFilter );
		}
		#endif

	};




	class ResourceReference: public bfu::SerializableClassBase<ResourceReference>
	{
		char m_resourceType[16];

		SERIALIZABLE_OBJ( ResourceTracker, UniqueID, m_binaryResourceID );

		sharedPtr<IConsumerResourceReferenceBase> rendererTXTResource;
	public:
		ResourceReference(const char* ResourceType);
		~ResourceReference();

		#ifdef IS_EDITOR
		void OnGUI()
		{
			ImGUI.Label("Resource type: %s", m_resourceType);
			
			UniqueID resourceID;
			//TODO
			if( rendererTXTResource.OnGUI( &resourceID, m_resourceType ) ) //changed resourceID
			{
				m_binaryResourceID = resourceID;
				rendererTXTResource = requestConsumerResourceReference(resourceID);
				rendererTXTResource.OnUnload2Renderer();
				rendererTXTResource.OnLoad2Renderer();
			}

			//optional
			RenderTYPEpreview();
		};
		#endif


		virtual void PostDeserialize() override
		{
			//if( m_binaryResourceID==0 ) request empty dummy resource for sanity

			rendererTXTResource = requestConsumerResourceReference(m_binaryResourceID);

			if( rendererTXTResource!=nullptr )
				rendererTXTResource.OnUnload2Renderer();
			
			rendererTXTResource.OnLoad2Renderer();
		}
		
	};

}
#endif