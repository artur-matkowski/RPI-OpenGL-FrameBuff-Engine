#ifndef _H_SERIALIZEJSON
#define _H_SERIALIZEJSON
#include "SerializableVar.hpp"
#include "SerializableVarVector.hpp"



namespace asapgl
{


class SerializableToJSON: public SerializableClassBase
{

	//member name, member reference
	std::map<const char*, SerializableBase*> m_membersMap;

public:

	virtual void PushReferenceToMap(const char* memberName, SerializableBase* memberReference)
	{
		m_membersMap[memberName] = memberReference;
	}


	virtual std::string Serialize()
	{
		std::stringstream ret;

		ret << "{";

		for (auto it = m_membersMap.begin(); ; ) {
			ret << "\n\"" << it->first << "\":"  << it->second->Serialize();

			++it;

			if(it != m_membersMap.end())
			{
				ret << ",";
			}
			else
			{
				break;
			}
		}

		ret << "\n}";

		return ret.str();
	}
	virtual void Deserialize(const char* str, const int size)
	{
		
	}

};

}


#endif