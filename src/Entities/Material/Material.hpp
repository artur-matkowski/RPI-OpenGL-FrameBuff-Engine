#ifndef _H_Material
#define _H_Material
#include "PersistanceObjectReference.hpp"
#include "_ResourceProcessorsInclude.hpp"
#include "SerializableObject.hpp"


namespace asapi
{
	class Material: public SerializableObject<Material>
	{
		SERIALIZABLE_OBJ(Material, ResourceGLSLSharedReference, m_shader );
	public:
	};

	class MaterialReference: public PersistanceObjectReference<Material, MaterialReference>
	{
	public:
		static const char* GetPersistanceType(){ return ".mat"; }
	};



}

#endif