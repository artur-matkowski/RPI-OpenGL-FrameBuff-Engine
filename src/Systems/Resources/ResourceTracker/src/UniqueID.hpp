#ifndef _H_UniqueID
#define _H_UniqueID
#include <cstdint>
#include "bfu.hpp"

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
		UniqueID( const uint64_t& );
		UniqueID( UniqueID&& cp ) noexcept;
		~UniqueID(){};

		UniqueID& operator=(UniqueID&&);

		inline uint64_t ID() const
		{
			return ID64;
		}
		inline void SetID(const uint64_t id)
		{
			ID64 = id;
			m_ID = id;
		}

		inline UniqueID& operator=(const uint64_t id)
		{
			ID64 = id;
			m_ID = id;

			return *this;
		}

		inline UniqueID& operator=(const UniqueID& id)
		{
			ID64 = id.ID64;
			m_ID = id.m_ID;

			return *this;
		}

		inline bool operator==( const UniqueID& other )
		{
			return ID64 == other.ID64;
		}

		inline bool operator!=( const UniqueID& other )
		{
			return ID64 != other.ID64;
		}

		inline operator uint64_t() const
		{
			return ID64;
		}

		bool operator<( const UniqueID& );
		bool operator>( const UniqueID& );

		virtual void PreSerializationCallback() override;
		virtual void PostDeserializationCallback() override;
	};
}

#endif