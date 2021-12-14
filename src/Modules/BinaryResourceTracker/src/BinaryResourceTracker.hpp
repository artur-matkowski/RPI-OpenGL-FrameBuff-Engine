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
		SERIALIZABLE_OBJ( ResourceTracker, UniqueID, m_resourceID );
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

		bool OnGUI( UniqueID& out_newResource )
		{
			//Render resource selector with type restriction
			//return true if resourcewas changes
		}
		
	};

	class RendererResource
	{
		//sharedPtr<
		BinaryResourceTracker binrestracker;

		void * rendererHandle = nullptr;

		void OnLoad2Renderer()
		{
			//TODO
		}

		void OnUnload2Renderer()
		{
			//TODO
		}

	};




	class ResourceReferenceTXT: public bfu::SerializableClassBase<ResourceReferenceTXT>
	{
		SERIALIZABLE_OBJ( ResourceTracker, UniqueID, m_resourceID );

		sharedPtr<RendererResource> rendererResource;
	public:
		ResourceReferenceTXT();
		~ResourceReferenceTXT();

		void OnGUI()
		{
			auto resourceID;
			//TODO
			if( binResTracker.OnGUI( &resourceID ) ) //changed resourceID
			{
				m_resourceID = resourceID;
				binrestracker = requestBinaryResourceTracker(resourceID);
				rendererResource.OnUnload2Renderer();
				rendererResource.OnLoad2Renderer();
			}

			//optional
			RenderTYPEpreview();
		};


		virtual OnDeserialize() override
		{
			binrestracker = requestBinaryResourceTracker(m_resourceID);

			if( rendererResource!=nullptr )
				rendererResource.OnUnload2Renderer();
			
			rendererResource.OnLoad2Renderer();
		}
		
	};

}
#endif