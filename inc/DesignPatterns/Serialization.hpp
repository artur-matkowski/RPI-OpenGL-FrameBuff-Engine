#ifndef _H_Serializable
#define  _H_Serializable
#include "object.hpp"
#ifndef _PLAYER
#include <bitforge/utils/bfu.hpp>	
#endif

namespace asapgl
{
	#ifdef _PLAYER
		#define SERIALIZABLR_CLASS_BASE							object

		#define SERIALIZABLE_VAR(TYPE) 							TYPE
		#define SERIALIZABLE_VAR_CONSTRUCTOR(NAME,PTR) 			NAME()

		#define SERIALIZABLE_VAR_VEC(TYPE) 						std::vector<TYPE>
		#define SERIALIZABLE_VAR_VEC_CONSTRUCTOR(NAME,PTR) 		NAME()


	
	#else
		#define SERIALIZABLR_CLASS_BASE							object, public bfu::SerializableClassBase

		#define SERIALIZABLE_VAR(TYPE) 							bfu::SerializableVar<TYPE>
		#define SERIALIZABLE_VAR_CONSTRUCTOR(NAME,PTR) 			NAME(#NAME, PTR)

		#define SERIALIZABLE_VAR_VEC(TYPE) 						bfu::SerializableVarVector<TYPE>
		#define SERIALIZABLE_VAR_VEC_CONSTRUCTOR(NAME,PTR) 		NAME(#NAME, PTR)
	#endif
}

#endif