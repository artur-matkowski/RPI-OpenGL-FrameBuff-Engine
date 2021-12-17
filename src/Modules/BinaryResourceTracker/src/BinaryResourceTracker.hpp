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
	class ConsumerResourceReference: public IConsumerResourceReferenceBase
	{
		#ifdef IS_EDITOR
		//sharedPtr<
		BinaryResourceTracker binrestracker;
		#endif

		void * rendererHandle = nullptr;

		ConsumerResourceReference();

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




	class ResourceReference: public bfu::SerializableClassBase<ResourceReference>
	{
		char m_resourceType[16];

		SERIALIZABLE_OBJ( ResourceTracker, UniqueID, m_binaryResourceID );

		sharedPtr<IConsumerResourceReferenceBase> rendererResource;
	public:
		ResourceReference(const char* ResourceType);
		~ResourceReference();

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