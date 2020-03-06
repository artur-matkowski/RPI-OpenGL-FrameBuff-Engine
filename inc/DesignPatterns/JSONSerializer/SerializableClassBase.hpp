#ifndef _H_SerializableClassBase
#define _H_SerializableClassBase
#include "PrecompiledHeaders.hpp"
#include "SerializableBase.hpp"

struct cmpByStringLength {
    bool operator()(const char* a, const char* b) const {
    	return std::strcmp(a, b) < 0;
    }
};

class SerializableClassBase: public SerializableBase
{
	//member name, member reference
	std::map<const char*, SerializableBase*, cmpByStringLength> m_membersMap;

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