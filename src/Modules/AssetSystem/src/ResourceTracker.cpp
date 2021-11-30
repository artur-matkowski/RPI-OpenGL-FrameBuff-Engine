#include "ResourceTracker.hpp"
#include "sha256.h"
#include "ResourceTrackerManager.hpp"

namespace asapi
{
	std::string ResourceTracker::_ResourceTrackersPath;

	ResourceTracker::ResourceTracker()
	{

	}
	ResourceTracker::~ResourceTracker()
	{
		if( m_outDated && m_resourceID.ID() != 0 )
		{
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
	}

	void ResourceTracker::SetProjectPath(const char* path)
	{
		_ResourceTrackersPath = path;
		if( _ResourceTrackersPath.back() != '/' )
			_ResourceTrackersPath += "/";
		_ResourceTrackersPath += "assets/Resource_Trackers/";
	}

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

	void ResourceTracker::Init(const char* path)
	{
		struct stat attrib;

		if( stat(path, &attrib)!=0 )
    	{
    		log::warning << "Could not find file " << path << std::endl;
    		return;
    	}

    	m_path = std::string(path);
    	const char* filename = path + m_path.size()-2;
    	for(; filename!=path && filename[0]!='/' ;--filename);
    	filename++;

    	m_filename = std::string(filename);

		m_content_hash = GetContentHash(path, &m_size);

		m_modified_epoch = static_cast<uint64_t>(attrib.st_mtim.tv_sec);
		m_modified_ns = static_cast<uint64_t>(attrib.st_mtim.tv_nsec);
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

		return *this;
	}
	void ResourceTracker::ObtainResourceOwnership(ResourceTracker & source)
	{
		m_resourceID = std::move(source.m_resourceID);
	}

	bool ResourceTracker::operator==(const ResourceTracker& other)
	{
		return m_path == other.m_path || m_content_hash == other.m_content_hash;
	}
	bool ResourceTracker::operator!=(const ResourceTracker& other)
	{
		return !this->operator==(other);
	}


	bool ResourceTracker::CmpContent(const ResourceTracker& other)
	{
		return m_content_hash == other.m_content_hash;
	}
	bool ResourceTracker::CmpPath(const ResourceTracker& other)
	{
		return m_path == other.m_path;
	}

	std::string ResourceTracker::GetResourceTrackerPath()
	{
		std::string ret = _ResourceTrackersPath;
		ret += std::to_string( m_resourceID.ID() );
		ret += ".res.json";

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
			//log::debug << "Removed ResourceTracker file: " << path.c_str() << std::endl;
		}
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
		return os;
	}
}