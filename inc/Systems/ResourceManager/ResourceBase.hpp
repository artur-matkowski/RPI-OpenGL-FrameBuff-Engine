#ifndef H_ResourceBase
#define H_ResourceBase
#include "object.hpp"
#include <bitforge/utils/bfu.hpp>

namespace asapi
{
	class ResourceBase: public object
	{
	protected:
		static char  				m_JSONbuff[2048];
		static bfu::JSONStream		m_JSONStream;
	};
}

#endif