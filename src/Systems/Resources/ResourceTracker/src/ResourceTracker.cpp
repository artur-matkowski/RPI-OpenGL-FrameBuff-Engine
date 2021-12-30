#include "ResourceTracker.hpp"
#include "ResourceTrackerManager.hpp"

#ifdef IS_EDITOR
#include "sha256.h"
#endif

namespace asapi
{
	std::string ResourceTracker::_ResourceTrackersPath;
	std::string ResourceTracker::_ProjectPath;


	SerializableSubResourceData::SerializableSubResourceData()
	{

	}
	SerializableSubResourceData::SerializableSubResourceData( const SubResourceData& in_data )
	{
		m_internalID = in_data.m_internalID;
		m_resourceID = in_data.m_resourceID;
	}
	SerializableSubResourceData::SerializableSubResourceData( SerializableSubResourceData&& in_data )
	{
	}
	SerializableSubResourceData& SerializableSubResourceData::operator=( SubResourceData&& in_data )
	{
		m_internalID = std::move( in_data.m_internalID );
		m_resourceID = std::move( in_data.m_resourceID );

		return *this;
	}

	ResourceTracker::ResourceTracker()
	{

	}
	ResourceTracker::~ResourceTracker()
	{
		if( IsResourceOwner() )
		{
			RemoveResourceBinaryFile();
			RemoveResourceTrackerFile();
		}
	}
	ResourceTracker::ResourceTracker(const ResourceTracker& cp)
	{
		Init( m_path.c_str() );
	}
	ResourceTracker::ResourceTracker(ResourceTracker&& cp) noexcept
	{
		m_resourceID = std::move(cp.m_resourceID);
		m_filename = std::move(cp.m_filename);
		m_path = std::move(cp.m_path);
		m_content_hash = std::move(cp.m_content_hash);
		m_size = std::move(cp.m_size);
		m_modified_epoch = std::move(cp.m_modified_epoch);
		m_modified_ns = std::move(cp.m_modified_ns);
		v_subresources = std::move(cp.v_subresources);

		m_resTrackerFilename = std::to_string( m_resourceID.ID() );
		m_resTrackerFilename += ".res.json";
	}

	void ResourceTracker::SetProjectPath(const char* path)
	{
		_ResourceTrackersPath = path;
		_ProjectPath = path;
		_ResourceTrackersPath += RESOURCE_TRACKERS_DIR;
	}

	#ifdef IS_EDITOR
	std::string ResourceTracker::GetContentHash(const char* path, uint32_t* out_size )
	{
		FILE::MMAP data;
        data.InitForRead(path);

		SHA256 sha256; 
		sha256(data.Data(), data.Size());

		std::string ret = sha256.getHash();
		*out_size = data.Size();

		return ret;
	}
	#endif


	void ResourceTracker::Init(const char* in_path)
	{
		struct stat attrib;

    	m_path = in_path;

		std::string path = _ProjectPath;
    	path += ASSETS_DIR;
    	path +="/";
    	path += in_path;

		if( stat(path.c_str(), &attrib)!=0 )
    	{
    		log::warning << "Could not find file " << path.c_str() << std::endl;
    		return;
    	}


    	const char* filename = path.c_str() + path.size()-2;
    	for(; filename!=path.c_str() && filename[0]!='/' ;--filename);
    	filename++;

    	m_filename = std::string(filename);

		#ifdef IS_EDITOR
		m_content_hash = GetContentHash(path.c_str(), &m_size);
		#endif

		m_modified_epoch = static_cast<uint64_t>(attrib.st_mtim.tv_sec);
		m_modified_ns = static_cast<uint64_t>(attrib.st_mtim.tv_nsec);

		
		m_resTrackerFilename = std::to_string( m_resourceID.ID() );
		m_resTrackerFilename += ".res.json";
	}


	ResourceTracker& ResourceTracker::operator=(const ResourceTracker& cp)
	{
		Init( m_path.c_str() );

		return *this;
	}
	ResourceTracker& ResourceTracker::operator=(ResourceTracker&& cp)
	{
		m_resourceID = std::move(cp.m_resourceID);
		m_filename = std::move(cp.m_filename);
		m_path = std::move(cp.m_path);
		m_content_hash = std::move(cp.m_content_hash);
		m_size = std::move(cp.m_size);
		m_modified_epoch = std::move(cp.m_modified_epoch);
		m_modified_ns = std::move(cp.m_modified_ns);

		m_resTrackerFilename = std::to_string( m_resourceID.ID() );
		m_resTrackerFilename += ".res.json";

		return *this;
	}
	void ResourceTracker::ObtainResourceOwnership(ResourceTracker & source)
	{
		m_resourceID = std::move(source.m_resourceID);
		v_subresources = std::move(source.v_subresources);

		m_resTrackerFilename = std::to_string( m_resourceID.ID() );
		m_resTrackerFilename += ".res.json";
	}

	bool ResourceTracker::operator==(const ResourceTracker& other) const
	{
		return m_path == other.m_path || m_content_hash == other.m_content_hash;
	}
	bool ResourceTracker::operator!=(const ResourceTracker& other) const
	{
		return !this->operator==(other);
	}


	bool ResourceTracker::CmpContent(const ResourceTracker& other) const
	{
		return m_content_hash == other.m_content_hash;
	} 
	bool ResourceTracker::CmpPath(const ResourceTracker& other) const
	{
		return m_path == other.m_path;
	}

	std::string ResourceTracker::GetResourceTrackerPath() const
	{
		std::string ret = _ResourceTrackersPath;
		ret += "/";
		ret += m_resTrackerFilename;

		return ret;
	}

	void ResourceTracker::RemoveResourceTrackerFile()
	{
		std::string path = GetResourceTrackerPath();
		int resoult = unlink( path.c_str() );

		if( resoult!=0 )
		{
			log::warning << "Can not remove ResourceTracker file: " << path.c_str() << std::endl;
		}
		else
		{
			//log::debug << "Removed ResourceTracker file: " << path.c_str()  << std::endl;
		}
	}

	void ResourceTracker::RemoveResourceBinaryFile()
	{
		const std::string binaryDirectory = _ProjectPath + RESOURCE_BINARIES_DIR;
		std::vector< std::string > paths;

		ListFiles(paths, {".bin",".json"}, ListingStrategy::whitelist, binaryDirectory.c_str() );

		for(int i=0; i<v_subresources.size(); ++i)
		{
			for(int k=0; k<paths.size(); ++k)
			{
				if( strstr(paths[k].c_str(), std::to_string(v_subresources[i].m_resourceID.ID()).c_str())!=0 )
				{
					std::string path = binaryDirectory + "/" + paths[k].c_str();
					int resoult = unlink( path.c_str() );

					if( resoult!=0 )
					{
						log::warning << "Can not remove Resource file: " << path.c_str() << std::endl;
					}
					else
					{
						//log::debug << "Removed Resource file: " << path.c_str() << std::endl;
					}
				}
			}
		}
	}


	const std::string ResourceTracker::GetFileExtension() const
	{
		std::string ret;

		for(int i=m_filename.size()-1; i>=0; --i)
		{
			if( m_filename[i]=='.' )
			{
				const char* extensionDot = m_filename.c_str() + i;
				ret = extensionDot;
				break;
			}
		}
		return ret;
	}

	void ResourceTracker::MoveSubresources(std::vector<SubResourceData>* in_subresources)
	{
		v_subresources.clear();
		for(int j=0; j<in_subresources->size(); ++j)
		{
			v_subresources.push_back( SerializableSubResourceData::AllocateAndInit( bfu::StdAllocatorMemBlock::GetMemBlock() ) );
			*(SerializableSubResourceData*)v_subresources.back() = std::move( (*in_subresources)[j] );
		}
	}

	bool ResourceTracker::FindSubResourceByInternalID( const std::string& in_id, asapi::UniqueID& out_ID ) const
	{
		out_ID = UniqueID();

		for(int i=0; i<v_subresources.size(); ++i)
		{
			if( strcmp( (*(SerializableSubResourceData*)*(v_subresources.begin()+i)).m_internalID.c_str(), in_id.c_str()) == 0 )
			{
				out_ID = (*(SerializableSubResourceData*)*(v_subresources.begin()+i)).m_resourceID;
				log::debug << "------------------------founded " << out_ID << std::endl;
				return true;
			}
		}

		return false;
	}

	bfu::stream& operator<<(bfu::stream& os, const ResourceTracker& res)
	{
		time_t mod = static_cast<time_t>( res.m_modified_epoch );

		char buff[128];
		strftime(buff, 128, "%D %T", gmtime(&mod));

		os << "\nResourceTracker printout:"; 
		os << "\n\tfilename: " << res.m_filename.c_str();
		os << "\n\tpath: " << res.m_path.c_str();
		os << "\n\tresource ID: " << res.m_resourceID.ID();
		os << "\n\tcontent hash: " << res.m_content_hash.c_str();
		os << "\n\tsize: " << res.m_size;
		os << "\n\tmodified: " << buff << ":" << res.m_modified_ns;

		os << "\n\tSubresources:";
		//for(int i=0; i<res.v_subresources.size(); ++i)
		{
			os << "\n" << res.v_subresources;
		}

		return os;
	}

	bfu::stream& operator<<(bfu::stream& os, const SerializableSubResourceData& res)
	{
		os << "\n\t\tResource ID     : " << res.m_resourceID.ID();
		os << "\n\t\tInternal ID     : " << res.m_internalID.c_str();

		return os;
	}
}