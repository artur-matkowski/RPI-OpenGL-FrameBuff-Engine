#ifndef H_ResourceSystem
#define H_ResourceSystem
#include <tuple>


namespace asapi
{

	template<int maxPathSize, class... Ts>
	class ResourceSystem
	{
		char 											m_projectPath[maxPathSize];
		std::tuple< std::map< UniqueID, Ts ...> >  		m_resources;



	public:
		ResourceSystem();
		~ResourceSystem();

		template<class T>
		ResourceSharedReference RequestResource( UniqueID resourceID )
		{
			auto it_resourceSearchResoult = std::get< std::map< UniqueID, T > >( m_resources ).find( resourceID );
			if( it_resourceSearchResoult != m_resources.end() )
				return ResourceSharedReference( *it_resourceSearchResoult );
			else
				return ResourceSharedReference();
		}

		bool SetProjectPath( const char* projectPath )
		{
			strncpy(m_projectPath, projectPath, maxPathSize);
		}

	};
}

#endif