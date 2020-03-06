#include "SerializableClassBase.hpp"
#include "JSONStream.hpp"


void SerializableClassBase::Serialize(JSONStream& stream)
{
	stream.sprintf("{");

	auto last = m_membersMap.end();

	for(auto it = m_membersMap.begin(); it != last; ++it)
	{
		stream.sprintf("\n\"%s\": ", it->first );

		it->second->Serialize( stream );
	}

	stream.sprintf("\n}");
}

void SerializableClassBase::Deserialize(JSONStream& stream)
{

}