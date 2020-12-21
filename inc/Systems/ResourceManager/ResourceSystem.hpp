#ifndef _H_ResourceManager
#define _H_ResourceManager

#include <map>
#include "ResourcePtr.hpp"

namespace asapgl
{

	class ResourceSystem
	{
		std::map<std::string, ResourcePtr<int> > m_map;

	public:

		bool requestResource(ResourcePtr<int>* res, const char* str)
		{
			std::string id(str);

			std::map<std::string, ResourcePtr<int> >::iterator it = m_map.find(id);

			if( it==m_map.end() )
			{
				res->Rebuild( new int(156) );
				m_map[id] = *res;
			}
			else
			{
				*res = m_map[id];
			}
			
			return true;
		}

		void dispouseResource(ResourcePtr<int>* res)
		{
			for(std::map<std::string, ResourcePtr<int> >::iterator it = m_map.begin() ;
				it!=m_map.end();
				++it)
			{
				void* A = it->second.GetRawPtr();
				void* B = res->GetRawPtr();

				if( A == B )
				{
					m_map.erase(it);
					break;
				}
			}
		}
	};

}


#endif