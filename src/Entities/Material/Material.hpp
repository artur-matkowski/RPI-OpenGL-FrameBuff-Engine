#ifndef _H_Material
#define _H_Material
#include "PersistanceObjectReference.hpp"



namespace asapi
{
	class Material: public bfu::SerializableClassBase<Material>
	{
		SERIALIZABLE_VAR(Material, bool, m_boolV_inMaterialInstance );
	public:

	};

	class MaterialReference: public PersistanceObjectReference<Material, MaterialReference>
	{
	public:
		static const char* GetPersistanceType(){ return ".mat"; }
	};



}

#endif