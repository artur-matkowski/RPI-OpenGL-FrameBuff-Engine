#ifndef _H_ResourcePtr
#define _H_ResourcePtr
//#include "Systems.hpp"

template < typename T > class ResourcePtr
{
private:
    T*    	pData = 0;
    int 	buff = 1;
    int* 	reference = 0;

public:
    ResourcePtr()
   		:pData(0)
   		,reference(0)
   	{
	}
    ResourcePtr(T* pValue)
   		:pData(pValue)
   		,reference(&buff)
   	{
	}
    ResourcePtr(const ResourcePtr<T>& sp) 
    	:pData(sp.pData)
    	,reference(sp.reference)
    {
	    ++(*reference);
	}

	void Rebuild(T* pValue);

    ~ResourcePtr();

    T& operator* ()
	{
	    return *pData;
	}

    T* operator-> ()
	{
	    return pData;
	}

	void* GetRawPtr()
	{
		return (void*)pData;
	}

    
    ResourcePtr<T>& operator = (const ResourcePtr<T>& sp);



	template<typename C>
	ResourcePtr(const ResourcePtr<C>& sp) 
    	:pData((T*)sp.pData)
    	,reference(sp.reference)
    {
	    ++(*reference);
	}

	template<typename C>
	ResourcePtr<T>& operator = (const ResourcePtr<C>& sp);

	template<typename> friend class ResourcePtr;
};



#endif