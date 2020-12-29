#ifndef _H_UniqueID
#define _H_UniqueID
#include "Serialization.hpp"
#include <cstdint>

namespace asapgl
{
	class UniqueID: public SERIALIZABLR_CLASS_BASE
	{
		SERIALIZABLE_VAR(uint64_t) ID;

		union{
			uint64_t ID64;
			uint32_t ID32[2];
		};

	public:
		UniqueID();
		~UniqueID(){};

		
		#ifndef _PLAYER
			virtual void Serialize(bfu::JSONStream& stream);

			virtual void Deserialize(bfu::JSONStream& stream);
		#endif
	};
}

#endif