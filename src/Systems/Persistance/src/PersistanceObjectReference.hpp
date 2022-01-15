#ifndef H_PersistanceObjectReference
#define H_PersistanceObjectReference
#include "bfu.hpp"
#include "imgui.h"
#include "PersistanceSystem.hpp"
#include "ResourceTracker.hpp"

namespace asapi
{
	class PersistanceObjectBase
	{
		static std::string  					s_projectPath;
		static std::map<UniqueID, PersistanceObjectBase*>	
												s_persistanceObjectsMap;
	protected:
		static void OnGUI_select( UniqueID* out_newID, const UniqueID& in_oldId, const char* asset_type);
	public:
		static void SetProjectPath(const char* path);
		static void CleanUp();
	};



	template<class InstanceT, class ReferenceT>
	class PersistanceObjectReference: public bfu::SerializableClassBase< ReferenceT >, public PersistanceObjectBase
	{
		InstanceT* 								mp_instance = nullptr;
		uint16_t*								mp_referenceCounter = nullptr;
		SERIALIZABLE_OBJ(ReferenceT, UniqueID, m_PersistanceObjectID );
		int postdeserializationcalls = 0;

	public:
		PersistanceObjectReference(){};
		PersistanceObjectReference(const PersistanceObjectReference& cp);
		~PersistanceObjectReference();

		inline InstanceT* GetObjectInstance(){ return mp_instance; }

		static PersistanceObjectReference<InstanceT, ReferenceT> RequestPersistanceObjectReference(const UniqueID& id);

		inline void OnGUI();


		virtual void PostDeserializationCallback()		override;
	};


	template<class InstanceT, class ReferenceT>
	PersistanceObjectReference< InstanceT, ReferenceT >::PersistanceObjectReference(const PersistanceObjectReference& cp)
	{
		if( mp_referenceCounter!=nullptr )
		{
			*mp_referenceCounter--;
			if( (*mp_referenceCounter==0) && (mp_instance!=nullptr) )
			{
				delete mp_instance;
			}
		}

		mp_instance = cp.mp_instance;
		mp_referenceCounter = cp.mp_referenceCounter;

		mp_referenceCounter++;
	}

	template<class InstanceT, class ReferenceT>
	PersistanceObjectReference< InstanceT, ReferenceT >::~PersistanceObjectReference()
	{
		if( mp_referenceCounter!=nullptr )
		{
			*mp_referenceCounter--;
			if( (*mp_referenceCounter==0) && (mp_instance!=nullptr) )
			{
				delete mp_instance;
			}
		}
	}

	template<class InstanceT, class ReferenceT>
	PersistanceObjectReference<InstanceT, ReferenceT> PersistanceObjectReference< InstanceT, ReferenceT >::RequestPersistanceObjectReference(const UniqueID& id)
	{
		auto it = s_persistanceObjectsMap.find( id );

		PersistanceObjectReference<InstanceT, ReferenceT> ret;

		if( it==s_persistanceObjectsMap.end() )
		{
			std::string path = s_projectPath + RESOURCE_BINARIES_DIR "/";
			path += std::to_string( id.ID() ) + InstanceT::GetPersistanceType();
			path += ".json";

			ret.mp_instance = new InstanceT();
			ret.mp_referenceCounter = new uint16_t;
			*(ret.mp_referenceCounter) = 1;

			PersistanceSystem::Deserialize( path.c_str(), ret.mp_instance );

			s_persistanceObjectsMap[ id ] = ret;
		}
		else
		{
			ret = it->second;
			*(ret.mp_referenceCounter)++;
		}

		return ret;
	}

	template<class InstanceT, class ReferenceT>
	void PersistanceObjectReference< InstanceT, ReferenceT >::OnGUI()
	{
		UniqueID newID;

		ImGui::Text("Persistance asset \"%s\" instance select:", ReferenceT::s_className );
		ImGui::Text("PostDeserializationCallback \"%d\" ", postdeserializationcalls );
		OnGUI_select( &newID, m_PersistanceObjectID, ReferenceT::GetPersistanceType() );

		if( newID.ID() != m_PersistanceObjectID.ID() )
		{
			m_PersistanceObjectID = newID;
			PostDeserializationCallback();
		}
	}


	template<class InstanceT, class ReferenceT>
	void PersistanceObjectReference< InstanceT, ReferenceT >::PostDeserializationCallback()
	{
		postdeserializationcalls++;
	}
}

#endif