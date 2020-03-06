#ifndef _H_SerializableVar
#define _H_SerializableVar
#include "JSONStream.hpp"
#include "SerializableClassBase.hpp"


template<class T>
class SerializableVar: public SerializableBase
{
	T m_var;
	
	SerializableVar()
	{}
public:

	SerializableVar(const T& val)
	{
		m_var = val;
	}

	SerializableVar(const char* Name, SerializableClassBase* parent)
	{
		if(parent!=0)
			parent->PushReferenceToMap(Name, this);
	}

	inline operator T() const
	{
		return m_var;
	}

	inline SerializableVar<T>& operator=(const T& val)
	{
		m_var = val;
		return *this;
	}

	virtual void Serialize(JSONStream& stream)
	{
		stream << m_var;
	}
	
	virtual void Deserialize(JSONStream& stream)
	{
		stream >> m_var;
	}
};

template<>
class SerializableVar<int>: public SerializableBase
{
	int m_var;
public:

	SerializableVar(const int& val)
	{
		m_var = val;
	}

	SerializableVar(const char* Name, SerializableClassBase* parent)
	{
		if(parent!=0)
			parent->PushReferenceToMap(Name, this);
	}

	inline operator int() const
	{
		return m_var;
	}

	inline SerializableVar<int>& operator=(const int& val)
	{
		m_var = val;
		return *this;
	}

	virtual void Serialize(JSONStream& stream)
	{
		stream.Serialize(m_var);
	}
	
	virtual void Deserialize(JSONStream& stream)
	{
		stream.Deserialize(m_var);
	}
};

template<>
class SerializableVar<float>: public SerializableBase
{
	float m_var;
public:

	SerializableVar(const float& val)
	{
		m_var = val;
	}

	SerializableVar(const char* Name, SerializableClassBase* parent)
	{
		if(parent!=0)
			parent->PushReferenceToMap(Name, this);
	}

	inline operator float() const
	{
		return m_var;
	}

	inline SerializableVar<float>& operator=(const float& val)
	{
		m_var = val;
		return *this;
	}

	virtual void Serialize(JSONStream& stream)
	{
		stream.Serialize(m_var);
	}
	
	virtual void Deserialize(JSONStream& stream)
	{
		stream.Deserialize(m_var);
	}
};

template<>
class SerializableVar<bool>: public SerializableBase
{
	bool m_var;
	char buff[6] = {0};
public:

	SerializableVar(const bool& val)
	{
		m_var = val;
	}

	SerializableVar(const char* Name, SerializableClassBase* parent)
	{
		if(parent!=0)
			parent->PushReferenceToMap(Name, this);
	}

	inline operator bool() const
	{
		return m_var;
	}

	inline SerializableVar<bool>& operator=(const bool& val)
	{
		m_var = val;
		return *this;
	}

	virtual void Serialize(JSONStream& stream)
	{
		stream.Serialize(m_var);
	}


	virtual void Deserialize(JSONStream& stream)
	{
		stream.Deserialize(m_var);
	}
};


template<>
class SerializableVar<std::string>: public SerializableBase, public std::string
{
	SerializableVar()
		:std::string()
		{};
public:

	SerializableVar(const std::string& val)
	{
		this->assign( val );
	}


	SerializableVar(const char* Name, SerializableClassBase* parent)
		:std::string()
	{
		if(parent!=0)
			parent->PushReferenceToMap(Name, this);
	}

	inline operator std::string() const
	{
		return *this;
	}

	inline SerializableVar<std::string>& operator=(const std::string& val)
	{
		this->assign(val);
		return *this;
	}

	inline SerializableVar<std::string>& operator=(const char* val)
	{
		this->assign(val);
		return *this;
	}


	virtual void Serialize(JSONStream& stream)
	{
		stream.Serialize( *(std::string*)this );
	}


	virtual void Deserialize(JSONStream& stream)
	{
		stream.Deserialize( *(std::string*)this );
	}
};



#endif