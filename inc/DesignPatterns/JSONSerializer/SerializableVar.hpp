#ifndef _H_SerializableVar
#define _H_SerializableVar
#include "JSONSerializerBase.hpp"

namespace asapgl
{

	template<class T>
	class SerializableVar: public SerializableBase
	{
		T m_var;
	public:

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

		virtual std::string Serialize()
		{
			std::stringstream ret;
			ret << m_var;
			return ret.str();
		}

		virtual void Deserialize(const char* str, const int size)
		{
			std::istringstream iss(str);

			iss >> m_var;
		}	
	};

	template<>
	class SerializableVar<bool>: public SerializableBase
	{
		bool m_var;
		char buff[6] = {0};
	public:

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

		virtual std::string Serialize()
		{
			std::stringstream ret;
			ret << (m_var ? "true" : "false");
			return ret.str();
		}

		virtual void Deserialize(const char* str, const int size)
		{
			std::istringstream iss(str);

			iss >> buff;

			if( 0 == std::strcmp("true", buff) )
			{
				m_var = true;
			}
			else if( 0 == std::strcmp("false", buff) )
			{
				m_var = false;
			}
			else
			{
				Debug::Trace(DebugLevel::WARNING) << "JSON Deserialize failed with bool string: " << str << std::endl;
			}
		}	
	};


	template<>
	class SerializableVar<std::string>: public SerializableBase, public std::string
	{
		SerializableVar()
			:std::string()
			{};
	public:

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


		virtual std::string Serialize()
		{
			std::stringstream ret;
			ret << "\"" << *this << "\"";
			return ret.str();
		}

		virtual void Deserialize(const char* str, const int size)
		{
			int length = 0;

			while( str[++length] != '\"' );

			this->assign(str+1, length-1);
		}	
	};
}

#endif