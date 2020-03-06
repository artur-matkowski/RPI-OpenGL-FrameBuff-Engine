#ifndef _H_SerializableBase
#define _H_SerializableBase

#define STRINGIFY(x) #x

class JSONStream;

class SerializableBase
{
	
public:
	virtual void Serialize(JSONStream& stream) = 0;

	virtual void Deserialize(JSONStream& stream) = 0;
};




#endif