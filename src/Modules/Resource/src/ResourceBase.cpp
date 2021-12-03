#include "ResourceBase.hpp"
#include "bfu.hpp"

namespace asapi
{
	ResourceBase::ResourceBase(uint64_t resourceIDlink)
		:m_resourceIDlink( resourceIDlink )
		,m_reference( new int(0) )
	{

	}
	ResourceBase::ResourceBase(const ResourceBase& cp)
		:m_resourceIDlink( cp.m_resourceIDlink )
		,m_reference( cp.m_reference )
	{
		if( *m_reference == 0 )
		{
			LoadResource();
		}

		++(*m_reference);
	}
	ResourceBase::~ResourceBase()
	{
		--(*m_reference);
	    if( *m_reference == 0 )
	    {
	        DispouseResource();
	    }
	    else if( *m_reference == -1 )
	    {
	    	delete m_reference;
	    }
	}


	bool ResourceBase::LoadResource()
	{
		log::warning << "Function overload not implemented for ResourceBase::LoadResource()" << std::endl;
		return false;
	}
	void ResourceBase::DispouseResource()
	{
		log::warning << "Function overload not implemented for ResourceBase::DispouseResource()" << std::endl;
	}
}