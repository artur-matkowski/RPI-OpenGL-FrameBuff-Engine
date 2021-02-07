#ifndef _H_UniqueID
#define _H_UniqueID
#include "Serialization.hpp"
#include "SerializationBinary.hpp"
#include <cstdint>

namespace asapgl
{
	
	// class UniqueID: public SERIALIZABLE_CLASS_BASE
	// {
	// 	SERIALIZABLE_VAR(uint64_t) ID;

	// 	union{
	// 		uint64_t ID64;
	// 		uint32_t ID32[2];
	// 	};

	// public:
	// 	UniqueID();
	// 	~UniqueID(){};

	// 	virtual void Serialize(std::ofstream& stream);
	// 	virtual void Deserialize(std::ifstream& stream);
	// };
}

#endif