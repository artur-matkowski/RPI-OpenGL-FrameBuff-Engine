#include "SerializableClassBase.hpp"
#include "JSONStream.hpp"





void SerializableClassBase::Serialize(JSONStream& stream)
{
	stream.sprintf("{");

	auto last = m_membersMap.end();

	for(auto it = m_membersMap.begin(); it != last; )
	{
		stream.sprintf("\n\"%s\": ", it->first );

		it->second->Serialize( stream );

		++it;

		if( it != last )
		{
			stream.sprintf(", ");
		}
	}

	stream.sprintf("\n}");
}

void SerializableClassBase::Deserialize(JSONStream& stream)
{
	stream.skipTo('{');
	stream.skip( 1 );

	std::string token;

	while( stream.peak() != '}' )
	{
		stream.Deserialize( token );

		m_membersMap[ token.c_str() ]->Deserialize( stream );

		stream.skipToOneOf("\"}");
	}
}