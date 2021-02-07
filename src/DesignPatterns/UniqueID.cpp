#include "UniqueID.hpp"
#include <stdlib.h>
#include <time.h> 

namespace asapgl
{
	
	// UniqueID::UniqueID()
	// 		:SERIALIZABLE_VAR_CONSTRUCTOR(ID,this)
	// {
	// 	ID32[0] = time(NULL);
	// 	ID32[1] = rand();
	// 	ID = ID64;
	// }

	// void UniqueID::Serialize(std::ofstream& stream)
	// {
	// 	ID = ID64;
	// 	stream << bits(ID);
	// }

	// void UniqueID::Deserialize(std::ifstream& stream)
	// {
	// 	stream >> bits(ID);
	// 	ID64 = ID;
	// }
}