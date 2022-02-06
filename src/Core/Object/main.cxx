#include "SerializableObject.hpp"

using namespace asapi;

class TestT: public SerializableObject<TestT>
{
public:
	SERIALIZABLE_VAR(TestT, float, b);

	/*virtual void OnGUI() override
	{
		printf("b is equal to %f", b);
	}*/
};

class TestTT: public SerializableObject<TestTT>
{
public:
	SERIALIZABLE_OBJ_VEC(TestTT, TestT, c);
};

int main(int argc, char** argv)
{
	TestTT* a = new TestTT();
	//object* c = a;
	//SerializableObject<TestT>* d = a;

	TestT* ptr1 = new TestT();
	TestT* ptr2 = new TestT();

	a->c.push_back(ptr1);
	a->c.push_back(ptr2);

	a->c[0].b = 3.14;
	a->c[1].b = 6.28;
/*
	printf("testt: %llu serializablebase: %llu object: %llu serializableobject<testt>: %llu\n"
		, (uint64_t) a
		, (uint64_t) b
		, (uint64_t) c
		, (uint64_t) d);
*/
	SerializableObjectBase* b = a;
	SerializableObjectBase* c = ptr1;

	a->OnGUI_caller();


	return 0;
}