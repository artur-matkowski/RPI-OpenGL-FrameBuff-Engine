#ifndef _H_SERIALIZEJSONBASE
#define _H_SERIALIZEJSONBASE
#include "PrecompiledHeaders.hpp"
#include "object.hpp"

#define STRINGIFY(x) #x

namespace asapgl
{
	class SerializableBase
	{
	public:
		virtual std::string Serialize() = 0;
		virtual void Deserialize(const char* str, const int size) = 0;
	};


	class SerializableClassBase: public SerializableBase
	{
	public:
		virtual void PushReferenceToMap(const char* memberName, SerializableBase* memberReference) = 0;
	};
}


#endif