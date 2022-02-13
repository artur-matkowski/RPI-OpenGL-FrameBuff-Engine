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

class TestTvT: public SerializableObject<TestTvT>
{
public:
	SERIALIZABLE_OBJ_VEC(TestTvT, TestT, c);
};


class TestToT: public SerializableObject<TestToT>
{
public:
	SERIALIZABLE_OBJ(TestToT, TestT, c);
};

int main(int argc, char** argv)
{
	TestTvT* a = new TestTvT();
	TestToT* ao = new TestToT();
	//object* c = a;
	//SerializableObject<TestT>* d = a;

	TestT* ptr1 = new TestT();
	TestT* ptr2 = new TestT();

	a->c.push_back(ptr1);
	a->c.push_back(ptr2);

	a->c[0].b = 3.14;
	a->c[1].b = 6.28;

	ao->c.b = 3.28;
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
	printf("\n---");
	ptr1->OnGUI_caller();
	printf("\n---");
	ao->OnGUI_caller();


	return 0;
}