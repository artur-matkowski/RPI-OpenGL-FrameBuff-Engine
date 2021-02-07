#ifndef _H_UniqueID
#define _H_UniqueID
#include <cstdint>
#include <bitforge/utils/bfu.hpp>

namespace asapgl
{
	class UniqueID: public bfu::SerializableClassBase
	{
		bfu::SerializableVar<uint64_t> m_ID;

		union{
			uint64_t ID64;
			uint32_t ID32[2];
		};

	public:
		UniqueID( bfu::MemBlockBase* mBlock );
		UniqueID( const UniqueID& mBlock );
		~UniqueID(){};

		virtual void Serialize(bfu::JSONStream& stream);
		virtual void Deserialize(bfu::JSONStream& stream);

		inline uint64_t ID()
		{
			return ID64;
		}
		inline void SetID(uint64_t id)
		{
			ID64 = id;
			m_ID = id;
		}
	};
}

#endif