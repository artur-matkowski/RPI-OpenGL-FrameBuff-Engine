#ifndef _H_SERIALIZEJSON
#define _H_SERIALIZEJSON
#include "PrecompiledHeaders.hpp"
#include "object.hpp"


#define STRINGIFY(x) #x
#define SERIALIZABLE_VAR(T,v)  SerializableVar<T> v(STRINGIFY(v))
#define SERIALIZABLE_VAR_VECTOR(T,v)  SerializableVarVector<T> v(STRINGIFY(v))

namespace asapgl
{

class SerializableBase
{};

template<class T>
class SerializableVar: public SerializableBase
{
	const char* m_Name;
	T m_var;
public:

	SerializableVar(const char* Name)
		:m_Name(Name)
	{}

	inline operator T() const
	{
		return m_var;
	}

	inline SerializableVar<T>& operator=(const T& val)
	{
		m_var = val;
		return *this;
	}

	inline std::string Serialize()
	{
		std::stringstream ret;
		ret << "\n\"" << m_Name << "\": << m_var << ";
		return ret.str();
	}

	inline SerializableVar<T>& Deserialize(const char* str, const int size)
	{
		int index = 0;

		for(int i=0; i<size; ++i)
		{
			if( str[i] == ':' )
			{
				index = i+2;
				break;
			}
		}
		std::istringstream iss(str+index);

		iss >> m_var;


		return *this;
	}	
};

template<class T>
class SerializableVarVector: public std::vector<T>
{
	const char* m_Name;
public:
	SerializableVarVector(const char* Name)
		:m_Name(Name)
		,std::vector<T>()
	{}

	inline std::string Serialize()
	{
		std::stringstream ret;
		ret << "\n\"" << m_Name << "\":[";

		int size = this->size();

		for(int i=0; i<size; ++i)
		{
			ret << this->operator[](i);

			if(i < size-1)
			{
				ret << ", ";
			}
		}

		ret << "]";

		return ret.str();
	}

	inline SerializableVarVector<T>& Deserialize(const char* str, const int size)
	{
		this->clear();

		int index = 0;

		for(int i=0; i<size; ++i)
		{
			if( str[i] == ':' )
			{
				index = i+2;
				break;
			}
		}

		T val;
		std::string tmp; 
		std::istringstream ss(str+index);
		std::vector<std::string> words;

		while(getline(ss, tmp, ',')){
			std::istringstream iss(tmp);

			iss >> val;

			this->push_back(val);
		}

		return *this;
	}
};

class SerializableToJSON{

	//member name, member reference
	std::map<const char*, SerializableBase*> m_membersMap;

public:

	template<class T>
	void PushReferenceToMap(const char* memberName, const char* typeName, T& memberReference)
	{

	}

};

}


#endif