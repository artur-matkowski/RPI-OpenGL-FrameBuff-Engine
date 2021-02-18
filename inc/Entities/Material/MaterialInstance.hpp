#ifndef _H_MaterialInstance
#define _H_MaterialInstance
#include "MaterialType.hpp"
#include "UniformOverride.hpp"

namespace asapi
{
	class MaterialInstance: public object//, public bfu::SerializableClassBase
	{
		//bfu::SerializableVar<std::string> 			m_usedMaterialType;

		ResourcePtr< MaterialType > 				m_materialType;

		std::map< std::string, UniformOverrideBase* >
													m_uniformOverridesMap;



	public:
		MaterialInstance();
		~MaterialInstance();

		
		inline void ObtainUniformOverride( const std::string& uniformName, UniformOverrideBase** out )
		{
			std::map< std::string, UniformOverrideBase* >::iterator 
				it(m_uniformOverridesMap.find(uniformName));

			if(it != m_uniformOverridesMap.end() )
			{
				*out = it->second;
			}
			else
			{
				UniformBase* materialTypeUniform = m_materialType->GetUniformPtr(uniformName);
				if( materialTypeUniform != 0 )
				{
					*out = materialTypeUniform->BuildUniformOverride(this);
				}
			}
		}
	};
}

#endif