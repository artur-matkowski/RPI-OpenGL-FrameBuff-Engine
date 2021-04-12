#ifndef _H_UniqueID
#define _H_UniqueID
#include <cstdint>
#include <bitforge/utils/bfu.hpp>

namespace asapi
{
	class UniqueID: public bfu::SerializableClassBase<UniqueID>
	{
		SERIALIZABLE_VAR(UniqueID, uint64_t, m_ID);

		union{
			uint64_t ID64;
			uint32_t ID32[2];
		};

	public:
		UniqueID();
		UniqueID( const UniqueID& cp );
		~UniqueID(){};

		inline uint64_t ID()
		{
			return ID64;
		}
		inline void SetID(uint64_t id)
		{
			ID64 = id;
			m_ID = id;
		}

		virtual void PreSerializationCallback() override;
		virtual void PostDeserializationCallback() override;
	};
}

#endif