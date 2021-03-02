#include "ResourceBase.hpp"
#include "Systems.hpp"

namespace asapi
{
	char  				ResourceBase::m_JSONbuff[2048];
	bfu::JSONStream		ResourceBase::m_JSONStream(ResourceBase::m_JSONbuff, 2048, SYSTEMS::SYSTEMS_ALLOCATOR );
}