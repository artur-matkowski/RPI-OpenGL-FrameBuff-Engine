#include "SerializableObject.hpp"

using namespace asapi;

class TestT: public SerializableObject<TestT>
{
public:
	SERIALIZABLE_VAR(TestT, float, b);

	virtual void OnGUI() override
	{
		printf("b is equal to %f", b);
	}
};

class TestTT: public SerializableObject<TestTT>
{
public:
	SERIALIZABLE_OBJ(TestTT, TestT, c);
};

int main(int argc, char** argv)
{
	TestTT* a = new TestTT();
	//SerializableObjectBase* b = a;
	//object* c = a;
	//SerializableObject<TestT>* d = a;

	a->c.b = 3.14;
/*
	printf("testt: %llu serializablebase: %llu object: %llu serializableobject<testt>: %llu\n"
		, (uint64_t) a
		, (uint64_t) b
		, (uint64_t) c
		, (uint64_t) d);
*/
	a->OnGUI_caller();


	return 0;
}