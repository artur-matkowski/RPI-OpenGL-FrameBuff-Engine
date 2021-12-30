#ifndef H_ResourceReference
#define H_ResourceReference
#include "BinaryResourceTracker.hpp"
#include "ResourceTrackerManager.hpp"
#include "object.hpp"

namespace asapi
{
	class ResourceSystemBase;

	class IResourceReferenceBase
	{
	protected:
		static std::string 				s_projectPath;
		static ResourceSystemBase* 		s_resourceSystem;
	public:
		inline static void SetProjectPath(const char* path) { s_projectPath = path; }
		inline static void SetResourceSystemReference( ResourceSystemBase* input ) { s_resourceSystem = input; }
		static BinaryResourceTracker* RequestBinaryResourceTracker( UniqueID );


	protected:
		void * 					m_rendererHandle = nullptr;
		uint16_t 				m_referenceCounter = 0;

	public:
		void IncreaseReferenceCounter();
		void DecreaseReferenceCounter();

		int GetReferenceCount() const { return m_referenceCounter; }


		#ifdef IS_EDITOR
		virtual bool OnGUI( UniqueID* out_newResource ) = 0;
		#endif
	};

	template<class T>
	class ResourceReference: public IResourceReferenceBase
	{
		#ifdef IS_EDITOR
		BinaryResourceTracker* m_binrestracker = nullptr;
		#endif

	public:
		ResourceReference( const UniqueID& id)
		{
			#ifdef IS_EDITOR
			if( m_binrestracker!=0 && id==m_binrestracker->m_binaryResourceID )
			{
				log::warning << "Traying to reaply the same resource. That makes no sense, check your code." << std::endl;
				return;
			}
			m_binrestracker = RequestBinaryResourceTracker( id );
			#endif

			char buff[MAX_PATH_SIZE];
			snprintf(buff, MAX_PATH_SIZE, "%s" RESOURCE_BINARIES_DIR "/%llu%s.bin", s_projectPath.c_str(), id.ID(), T::GetBinaryOutputFileExtension());
			
			m_rendererHandle = T::LoadResource( buff );
		}

		ResourceReference(ResourceReference&& cp)
		{
			m_rendererHandle = cp.m_rendererHandle;
			m_referenceCounter = cp.m_referenceCounter;
			cp.m_referenceCounter = -1;

			#ifdef IS_EDITOR
			m_binrestracker = cp.m_binrestracker;
			cp.m_binrestracker = 0;
			#endif
		}

		~ResourceReference()
		{
			if( m_referenceCounter>0 )
			{
				log::error << "Destroying ResourceReference while resource still in use. Program might soon crash." << std::endl;
			}
			if( m_referenceCounter==0 ) 
				T::UnloadResource( m_rendererHandle );
		}

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
		
		#ifdef IS_EDITOR
		virtual bool OnGUI( UniqueID* out_newResource ) override
		{
			//return T::OnGUI( out_newResource );
			return false;
		}
		#endif

	};
}

#endif