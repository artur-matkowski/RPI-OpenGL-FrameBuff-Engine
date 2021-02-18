#define _H_MaterialInterface
#define _H_MaterialInterface
#include "object.hpp"

namespace asapi
{
	class MaterialInterface: public object
	{
	protected:
		std::vector<UniformBase*>					m_dirtyUniforms;
		bool 										m_isDirty = false;

	public:
		MaterialInterface(){};
		~MaterialInterface(){};	

		inline void SetUniformDirty(UniformBase* dirty)
		{
			m_dirtyUniforms.push_back( dirty );
			m_isDirty = true;
		}
	};
}

#endif