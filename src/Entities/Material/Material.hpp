#ifndef _H_Material
#define _H_Material
#include "PersistanceObjectReference.hpp"
#include "_ResourceProcessorsInclude.hpp"



namespace asapi
{
	class Material: public bfu::SerializableClassBase<Material>
	{
		SERIALIZABLE_GUI_OBJ(Material, ResourceGLSLSharedReference, m_shader );
	public:
		void OnGUI(){};
	};

	class MaterialReference: public PersistanceObjectReference<Material, MaterialReference>
	{
	public:
		static const char* GetPersistanceType(){ return ".mat"; }
	};



}

#endif