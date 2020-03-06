#ifndef _H_TESTS
#define _H_TESTS

#include "Context.hpp"
#include "Systems.hpp"
#include "JSONStream.hpp"


using namespace asapgl;

class testClass2: public SerializableClassBase
{
public:
	SerializableVar<bool> m_var;
	SerializableVar<float> m_var2;
	SerializableVarVector<float> m_var3;



public:
	testClass2()
		:SerializableClassBase()
		,m_var("m_var",this)
		,m_var2("m_var2",this)
		,m_var3("m_var3",this)
	{
		m_var = true;
		m_var2 = 2.00123;

		m_var3.push_back(1.2);
		m_var3.push_back(1.1);
	}

	testClass2(const testClass2& copy)
		:SerializableClassBase()
		,m_var("m_var",this)
		,m_var2("m_var2",this)
		,m_var3("m_var3",this)
	{
		m_var = true;
		m_var2 = 2.00123;

		m_var3.push_back(1.2);
		m_var3.push_back(1.1);
	}
};


class testClass: public SerializableClassBase
{
public:
	SerializableVar<bool> m_var;
	SerializableVar<float> m_var2;
	SerializableVarVector<float> m_var3;

	SerializableVar<testClass2> m_var4;
	SerializableVarVector<testClass2> m_var5;



public:
	testClass()
		:SerializableClassBase()
		,m_var("m_var",this)
		,m_var2("m_var2",this)
		,m_var3("m_var3",this)
		,m_var4("m_var4",this)
		,m_var5("m_var5",this)
	{
		m_var = true;
		m_var2 = 2.00123;

		m_var3.push_back(1.2);
		m_var3.push_back(1.1);
		m_var3.push_back(3.2);
		m_var3.push_back(13.2);


		m_var5.push_back( testClass2() );
		m_var5.push_back(testClass2());
		m_var5.push_back(testClass2());
	}
};



template<typename T>
bool _TESTJSONStream(const char* _typename, const T& val)
{
	JSONStream json;
	JSONStream json2;
	SerializableVar<T> tt(_typename, 0);
	SerializableVar<T> tt2(_typename, 0);
	tt = val;

	json << tt;

	json.SetCursonPos(0);

	json >> tt2;
	json2 << tt2;

	Debug::Trace(DebugLevel::INFO) << "Testing: " << _typename << "\n\tOriginal input:\n\t\t>" << tt 
	 		<< "<\n\tSerialized to JSON:\n\t\t>" << json.str()  
	 		<< "<\n\tDeserialized back to type:\n\t\t>" << tt2
	 		<< "<\n\tSerialized to JSON2:\n\t\t>" << json2.str()  
			<< "<\n" << std::endl;

	if( std::strcmp(json.str().c_str(), json2.str().c_str() )==0 )
	{
		Debug::Trace(DebugLevel::WARNING) << "<<<<<<<<<<<<<<<< Test concluded : SUCCES\n" << std::endl;
		return true;
	}
	else
	{
		Debug::Trace(DebugLevel::ERROR) << "<<<<<<<<<<<<<<<< Test concluded : FAILED\n" << std::endl;
		return false;		
	}
}

#define TESTJSONStream(T,v) _TESTJSONStream<T>(#T, v)


template<typename T>
bool _TESTJSONStreamVector(const char* _typename, const std::vector<T> input)
{
	JSONStream json;
	JSONStream json2;

	SerializableVarVector<T> var("var",0);
	SerializableVarVector<T> var2("var2",0);
	var = input;

	json << var;

	json.SetCursonPos(0);

	json >> var2;
	json2 << var2;

	Debug::Trace(DebugLevel::INFO) << "Testing: " << _typename
	 		<< "<\n\tSerialized to JSON:\n\t\t>" << json.str()  
	 		<< "<\n\tSerialized to JSON:\n\t\t>" << json2.str()  
			<< "<\n" << std::endl;


	if( std::strcmp(json.str().c_str(), json2.str().c_str() )==0 )
	{
		Debug::Trace(DebugLevel::WARNING) << "<<<<<<<<<<<<<<<< Test concluded : SUCCES\n" << std::endl;
		return true;
	}
	else
	{
		Debug::Trace(DebugLevel::ERROR) << "<<<<<<<<<<<<<<<< Test concluded : FAILED\n" << std::endl;		
		return false;		
	}
}

#define TESTJSONStreamVector(T,...) _TESTJSONStreamVector<T>( #T , {__VA_ARGS__})

#endif