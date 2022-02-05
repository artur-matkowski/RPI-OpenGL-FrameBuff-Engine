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

		virtual ~PersistanceObjectBase() {};
	};



	template<class InstanceT, class ReferenceT>
	class PersistanceObjectReference: public bfu::SerializableClassBase< ReferenceT >, public PersistanceObjectBase
	{
		InstanceT* 								mp_instance = nullptr;
		uint16_t*								mp_referenceCounter = nullptr;
		SERIALIZABLE_OBJ(ReferenceT, UniqueID, m_PersistanceObjectID );

		void CleanUp();

	public:
		PersistanceObjectReference(){};
		PersistanceObjectReference(const PersistanceObjectReference& cp);
		virtual ~PersistanceObjectReference() override;

		PersistanceObjectReference<InstanceT, ReferenceT>& operator=(const PersistanceObjectReference& cp);

		inline InstanceT* GetObjectInstance(){ return mp_instance; }

		inline void OnGUI();


		virtual void PostDeserializationCallback()		override;


	};







	template<class InstanceT, class ReferenceT>
	void PersistanceObjectReference< InstanceT, ReferenceT >::CleanUp()
	{
		if( mp_referenceCounter!=nullptr )
		{
			(*mp_referenceCounter)--;
			if( (*mp_referenceCounter)==0 )
			{
				if(mp_instance!=nullptr)
				{
					delete mp_instance;
					mp_instance = nullptr;
				}

				delete mp_referenceCounter;
				mp_referenceCounter = nullptr;

				s_persistanceObjectsMap.erase( m_PersistanceObjectID );
			}
		}
	}

	template<class InstanceT, class ReferenceT>
	PersistanceObjectReference< InstanceT, ReferenceT >::PersistanceObjectReference(const PersistanceObjectReference& cp)
	{
		*this = cp;
	}

	template<class InstanceT, class ReferenceT>
	PersistanceObjectReference< InstanceT, ReferenceT >::~PersistanceObjectReference()
	{
		CleanUp();
	}

	template<class InstanceT, class ReferenceT>
	PersistanceObjectReference<InstanceT, ReferenceT>& PersistanceObjectReference< InstanceT, ReferenceT >::operator=(const PersistanceObjectReference& cp)
	{
		CleanUp();

		mp_instance = cp.mp_instance;
		mp_referenceCounter = cp.mp_referenceCounter;

		if( mp_referenceCounter!=nullptr)
			(*mp_referenceCounter)++;

		return *this;
	}


	template<class InstanceT, class ReferenceT>
	void PersistanceObjectReference< InstanceT, ReferenceT >::OnGUI()
	{
		UniqueID newID;

		ImGui::Text("Persistance asset \"%s\" instance select:", ReferenceT::s_className );
		OnGUI_select( &newID, m_PersistanceObjectID, ReferenceT::GetPersistanceType() );

		if( newID.ID() != m_PersistanceObjectID.ID() )
		{
			m_PersistanceObjectID = newID;
			PostDeserializationCallback();
		}

		if( ImGui::Button("Serialize Asset Instance") && mp_instance!=nullptr )
		{
			std::string path = s_projectPath + RESOURCE_BINARIES_DIR "/";
			path += std::to_string( m_PersistanceObjectID.ID() ) + ReferenceT::GetPersistanceType();
			path += ".json";

			PersistanceSystem::Serialize( path.c_str(), mp_instance );
		}

		if( mp_instance!=nullptr )
		{
			mp_instance->OnGUI();
		}
		else
		{
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "No valid reference for %s asset", ReferenceT::GetPersistanceType() );
		}
	}


	template<class InstanceT, class ReferenceT>
	void PersistanceObjectReference< InstanceT, ReferenceT >::PostDeserializationCallback()
	{
		CleanUp();

		auto it = s_persistanceObjectsMap.find( m_PersistanceObjectID );

		if( it==s_persistanceObjectsMap.end() )
		{
			std::string path = s_projectPath + RESOURCE_BINARIES_DIR "/";
			path += std::to_string( m_PersistanceObjectID.ID() ) + ReferenceT::GetPersistanceType();
			path += ".json";

			mp_instance = new InstanceT();
			mp_referenceCounter = new uint16_t;
			(*mp_referenceCounter) = 1;

			PersistanceSystem::Deserialize( path.c_str(), mp_instance );

			s_persistanceObjectsMap[ m_PersistanceObjectID ] = this;
		}
		else
		{
			PersistanceObjectReference< InstanceT, ReferenceT >* ptr = (PersistanceObjectReference< InstanceT, ReferenceT >*)it->second;
			*this = *ptr;
		}
	}
}

#endif