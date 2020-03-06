#ifndef _H_SerializableClassBase
#define _H_SerializableClassBase
#include "PrecompiledHeaders.hpp"
#include "SerializableBase.hpp"

class SerializableClassBase: public SerializableBase
{
	//member name, member reference
	std::map<const char*, SerializableBase*> m_membersMap;

	friend class JSONStream;

	//you need to have a copy constructor in your class for that to work
	//virtual SerializableClassBase(const SerializableClassBase& ) = 0;

public:

	virtual void Serialize(JSONStream& stream);

	virtual void Deserialize(JSONStream& stream);

	virtual void PushReferenceToMap(const char* memberName, SerializableBase* memberReference)
	{
		m_membersMap[memberName] = memberReference;
	}
};


#endif