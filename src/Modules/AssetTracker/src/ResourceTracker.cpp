#include "ResourceTracker.hpp"
#include "sha256.h"

namespace asapi
{
	ResourceTracker::ResourceTracker()
	{

	}
	ResourceTracker::~ResourceTracker()
	{

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

        data.InitForRead(path);

		SHA256 sha256; 
		sha256(data.Data(), data.Size());

		m_content_hash = sha256.getHash();
		m_size = data.Size();
		m_modified_epoch = static_cast<uint64_t>(attrib.st_mtim.tv_sec);
		m_modified_ns = static_cast<uint64_t>(attrib.st_mtim.tv_nsec);
	}

	bool ResourceTracker::operator==(const ResourceTracker& other)
	{
		return false;
	}


	bfu::stream& operator<<(bfu::stream& os, const ResourceTracker& res)
	{
		time_t mod = static_cast<time_t>( res.m_modified_epoch );

		char buff[128];
		strftime(buff, 128, "%D %T", gmtime(&mod));

		os << "ResourceTracker printout:"; 
		os << "\n\tpath: " << res.m_path.c_str(); 
		os << "\n\tcontent hash: " << res.m_content_hash.c_str(); 
		os << "\n\tsize: " << res.m_size; 
		os << "\n\tmodified: " << buff << ":" << res.m_modified_ns; 
		return os;
	}
}