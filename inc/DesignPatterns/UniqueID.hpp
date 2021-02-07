#ifndef _H_UniqueID
#define _H_UniqueID
#include <cstdint>
#include <bitforge/utils/bfu.hpp>

namespace asapgl
{
	class UniqueID: public bfu::SerializableClassBase
	{
		bfu::SerializableVar<uint64_t> ID;

		union{
			uint64_t ID64;
			uint32_t ID32[2];
		};

	public:
		UniqueID( bfu::MemBlockBase* mBlock );
		~UniqueID(){};

		virtual void Serialize(bfu::JSONStream& stream);
		virtual void Deserialize(bfu::JSONStream& stream);
	};
}

#endif