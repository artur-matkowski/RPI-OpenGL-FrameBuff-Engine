#ifndef _H_TESTS
#define _H_TESTS

#include "Context.hpp"
#include "Systems.hpp"
#include <SerializeJSON.hpp>


using namespace asapgl;

class testClass: public SerializableToJSON
{
public:
	SerializableVar<bool> m_var;
	SerializableVar<float> m_var2;
	SerializableVarVector<float> m_var3;



public:
	testClass()
		:m_var("m_var",this)
		,m_var2("m_var2",this)
		,m_var3("m_var3",this)
	{
		m_var = true;
		m_var2 = 2.00123;

		m_var3.push_back(1.2);
		m_var3.push_back(1.1);
		m_var3.push_back(3.2);
		m_var3.push_back(13.2);
	}
};


template<typename T>
void _TESTSerializableVar(const T& val, const char* _typename)
{
	SerializableVar<T> var("var",0);
	SerializableVar<T> var2("var2",0);

	var = val;

	std::string json = var.Serialize();

	Debug::Trace(DebugLevel::INFO) << "testing SerializableVar " << _typename << " Serialize()> " << var.Serialize() << std::endl;

	var2.Deserialize(json.c_str(), json.size());

	Debug::Trace(DebugLevel::INFO) << "testing SerializableVar " << _typename << " Deserialize()> " << var2.Serialize() << std::endl;

	std::string json2 = var2.Serialize();

	if( std::strcmp(json.c_str(),json2.c_str())==0 )
	{
		Debug::Trace(DebugLevel::WARNING) << "<<<<<<<<<<<<<<<< Test concluded : SUCCES\n" << std::endl;
	}
	else
	{
		Debug::Trace(DebugLevel::ERROR) << "<<<<<<<<<<<<<<<< Test concluded : FAILED\n" << std::endl;		
	}
}

#define TESTSerializableVar(T,v) _TESTSerializableVar<T>(v, #T)




template<typename T>
void _TESTSerializableVarVector(const char* _typename, const std::vector<T> input)
{
	SerializableVarVector<T> var("var",0);
	SerializableVarVector<T> var2("var2",0);

	var = input;

	std::string json = var.Serialize();

	Debug::Trace(DebugLevel::INFO) << "testing SerializableVarVector " << _typename << " Serialize()> " << var.Serialize() << std::endl;

	var2.Deserialize(json.c_str(), json.size());

	Debug::Trace(DebugLevel::INFO) << "testing SerializableVarVector " << _typename << " Deserialize()> " << var2.Serialize() << std::endl;

	std::string json2 = var2.Serialize();

	if( std::strcmp(json.c_str(),json2.c_str())==0 )
	{
		Debug::Trace(DebugLevel::WARNING) << "<<<<<<<<<<<<<<<< Test concluded : SUCCES\n" << std::endl;
	}
	else
	{
		Debug::Trace(DebugLevel::ERROR) << "<<<<<<<<<<<<<<<< Test concluded : FAILED\n" << std::endl;		
	}
}

#define TESTSerializableVarVector(T,...) _TESTSerializableVarVector<T>( #T , {__VA_ARGS__})


#endif