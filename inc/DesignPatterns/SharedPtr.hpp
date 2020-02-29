#ifndef H_SharedPtr
#define H_SharedPtr


template < typename T > class SharedPtr
{
private:
    T*    pData = 0;
    int* reference = 0;

public:
    SharedPtr()
   		:pData(0)
   		,reference(0)
   	{}
    SharedPtr(T* pValue)
   		:pData(pValue)
   		,reference(new int(1))
   	{}
    SharedPtr(const SharedPtr<T>& sp) 
    	:pData(sp.pData)
    	,reference(sp.reference)
    {
	    ++(*reference);
	}

    ~SharedPtr()
	{
		if(reference!=0)
		{
			--(*reference);
		    if( *reference == 0 )
		    {
		        delete pData;
		        delete reference;
		    }
		}	    
	}

    T& operator* ()
	{
	    return *pData;
	}

    T* operator-> ()
	{
	    return pData;
	}

    
    SharedPtr<T>& operator = (const SharedPtr<T>& sp)
	{
	    if (this != &sp)
	    {
	    	if( reference != 0 )
	    	{
	            --(*reference);
		        if( *reference == 0 )
		        {
		            delete pData;
		            delete reference;
		        }
	    	}

	        pData = sp.pData;
	        reference = sp.reference;
	        ++(*reference);
	    }
	    return *this;
	}



	template<typename C>
	SharedPtr(const SharedPtr<C>& sp) 
    	:pData((T*)sp.pData)
    	,reference(sp.reference)
    {
	    ++(*reference);
	}

	template<typename C>
	SharedPtr<T>& operator = (const SharedPtr<C>& sp)
	{
	    if (this != &sp)
	    {
	    	if( reference != 0 )
	    	{
	            --(*reference);
		        if( *reference == 0 )
		        {
		            delete pData;
		            delete reference;
		        }
	    	}

	        pData = (T*)sp.pData;
	        reference = sp.reference;
	        ++(*reference);
	    }
	    return *this;
	}

	template<typename> friend class SharedPtr;
};


#endif
