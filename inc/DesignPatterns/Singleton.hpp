#ifndef _H_SINGLETON
#define _H_SINGLETON

template<class T>
class Singleton{
	static void* _this;
public:
	Singleton(){};
	~Singleton(){};

	static T& GetObject()
	{
		if(_this==0)
			_this = new T();
		return *(T*)_this;
	}


};



template<class T>
void* Singleton<T>::_this = 0;

#endif