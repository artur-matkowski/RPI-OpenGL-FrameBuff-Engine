#ifndef H_ResourceReference
#define H_ResourceReference
#include "BinaryResourceTracker.hpp"
#include "ResourceTrackerManager.hpp"
#include "object.hpp"
#ifdef IS_EDITOR
#include "imgui.h"
#include <vector>
#endif

namespace asapi
{
	class ResourceSystemBase;

	class IResourceReferenceBase
	{
	protected:
		static std::string 				s_projectPath;
		static ResourceSystemBase* 		s_resourceSystem;

		#ifdef IS_EDITOR
		static std::vector<IResourceReferenceBase*> 	s_resourceReferences;
		#endif

	public:
		inline static void SetProjectPath(const char* path) { s_projectPath = path; }
		inline static void SetResourceSystemReference( ResourceSystemBase* input ) { s_resourceSystem = input; }
		static BinaryResourceTracker* RequestBinaryResourceTracker( UniqueID );
		inline void* GetRawHandle() const { return m_rawHandle; }


		#ifdef IS_EDITOR
		typedef void (*t_callback)(void* data);
		t_callback 										m_callback = nullptr;
		void* 											m_callbackData = nullptr;

		void BindOnDirtyCallback(t_callback callback, void* callbackData){ m_callback = callback; m_callbackData = callbackData;}
		static void RefreshResourceReference();

		#endif


	protected:
		void * 					m_rawHandle = nullptr;
		uint16_t 				m_referenceCounter = 0;

	public:
		void IncreaseReferenceCounter();
		void DecreaseReferenceCounter();

		int GetReferenceCount() const { return m_referenceCounter; }

		#ifdef IS_EDITOR
		void OnGUI()
		{
			ImGui::Text("\tResource stats:");
			ImGui::Text("\tReference counter: %d", m_referenceCounter);
			ImGui::Text("\tRawHandle: %llu", (uint64_t)m_rawHandle);
		}

		virtual void ReloadResourceReference() = 0;
		#endif
	};










	template<class T>
	class ResourceReference: public IResourceReferenceBase
	{
	protected:
		#ifdef IS_EDITOR
		BinaryResourceTracker* 							m_binrestracker = nullptr;
		UniqueID 										m_resourceId = 0;
		uint64_t 										m_modificationDate = 0;
		#endif

	public:

		ResourceReference( const UniqueID& id)
		{
			#ifdef IS_EDITOR
			if( m_binrestracker!=0 && id==m_binrestracker->GetSubResourceID() )
			{
				log::warning << "Traying to reaply the same resource. That makes no sense, check your code." << std::endl;
				return;
			}
			m_binrestracker = RequestBinaryResourceTracker( id );

			s_resourceReferences.push_back( this );

			m_resourceId = id;
			#endif

			char buff[MAX_PATH_SIZE];
			snprintf(buff, MAX_PATH_SIZE, "%s" RESOURCE_BINARIES_DIR "/%llu%s.bin", s_projectPath.c_str(), id.ID(), T::GetBinaryOutputFileExtension());
			
			#ifdef IS_EDITOR
			m_modificationDate = FILE::GetModificationTime( buff );
			#endif

			m_rawHandle = T::LoadResource( buff );
		}

		ResourceReference(ResourceReference&& cp)
		{
			m_rawHandle = cp.m_rawHandle;
			m_referenceCounter = cp.m_referenceCounter;
			cp.m_referenceCounter = -1;

			#ifdef IS_EDITOR
			m_binrestracker = cp.m_binrestracker;
			cp.m_binrestracker = 0;
			
			m_resourceId = cp.m_resourceId;
			cp.m_resourceId = 0;

			m_modificationDate = cp.m_modificationDate;
			cp.m_modificationDate = 0;

			for(int i=0; i<s_resourceReferences.size(); ++i)
			{
				if( s_resourceReferences[i]==&cp )
				{
					s_resourceReferences[i] = this;
					break;
				}
			}
			#endif
		}

		~ResourceReference()
		{
			if( m_referenceCounter>0 )
			{
				log::error << "Destroying ResourceReference while resource still in use. Program might soon crash." << std::endl;
			}
			if( m_referenceCounter==0 ) 
				T::UnloadResource( m_rawHandle );

			#ifdef IS_EDITOR
			for(auto it = s_resourceReferences.begin(); it!=s_resourceReferences.end(); it++)
			{
				if( (*it)==this )
				{
					s_resourceReferences.erase( it );
					break;
				}
			}
			#endif
		}


		#ifdef IS_EDITOR
		virtual void ReloadResourceReference() override
		{
			char buff[MAX_PATH_SIZE];
			snprintf(buff, MAX_PATH_SIZE, "%s" RESOURCE_BINARIES_DIR "/%llu%s.bin", s_projectPath.c_str(), m_resourceId.ID(), T::GetBinaryOutputFileExtension());
			
			uint64_t modificationDate = FILE::GetModificationTime( buff );
			const bool wasResourceModified = modificationDate != m_modificationDate;

			if(!wasResourceModified)
				return;


			T::UnloadResource( m_rawHandle );			
			m_rawHandle = T::LoadResource( buff );

			m_modificationDate = modificationDate;

			if( m_callback!=nullptr )
				m_callback( m_callbackData );
		}
		#endif

		static inline bool ProcessResource2Binary(const asapi::ResourceTracker& in_currentResource
											, asapi::FILE::MMAP* in_resourceFile
											, const char* in_projectPath
											, std::vector<asapi::SubResourceData>* out_resourceBinaries)
		{
			return T::ProcessResource2Binary(in_currentResource, in_resourceFile, in_projectPath, out_resourceBinaries);
		}

		static inline void GetSuportedFilesExtensions( std::vector<std::string>* out_suportedFileExtensions )
		{
			T::GetSuportedFilesExtensions( out_suportedFileExtensions );
		}

		inline uint16_t GetReferenceCounter()
		{
			return m_referenceCounter;
		}
	};
}

#endif