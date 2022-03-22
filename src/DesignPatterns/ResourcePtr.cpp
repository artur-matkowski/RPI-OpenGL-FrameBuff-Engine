#include "Texture.hpp"
#include "Shader.hpp"
#include "Mesh_old.hpp"

#include "ResourcePtr.hpp"
#include "Systems.hpp"

template < typename T > 
ResourcePtr<T>::~ResourcePtr()
{
	if(reference!=0)
	{
		--(*reference);
	    if( *reference == 1 )
	    {
	        delete pData;
	        asapi::SYSTEMS::GetObject().ASSETS.dispouseResource(this);
	        pData = 0;
	    }
	    else if( *reference == 0 )
	    {
	    	delete reference;
	    	reference = 0;
	    }
	    
	}	    
}


template < typename T > 
void ResourcePtr<T>::Rebuild(T* pValue)
{
	if( reference != 0 )
	{
        --(*reference);
	    if( *reference == 1 )
	    {
	        delete pData;
	        asapi::SYSTEMS::GetObject().ASSETS.dispouseResource(this);
	        pData = 0;
	    }
	    else if( *reference == 0 )
	    {
	    	delete reference;
	    	reference = 0;
	    }
	}

   	pData = pValue;
   	reference = new int(1);
}

template < typename T > 
ResourcePtr<T>& ResourcePtr<T>::operator = (const ResourcePtr<T>& sp)
{
    if (this != &sp)
    {
    	if( reference != 0 )
    	{
            --(*reference);
		    if( *reference == 1 )
		    {
		        delete pData;
	        asapi::SYSTEMS::GetObject().ASSETS.dispouseResource(this);
		        pData = 0;
		    }
		    else if( *reference == 0 )
		    {
	    		delete reference; 
	    		reference = 0;
		    }
    	}

        pData = sp.pData;
        reference = sp.reference;
        ++(*reference);
    }
    return *this;
}


template class ResourcePtr<asapi::Texture>;
template class ResourcePtr<asapi::Mesh_old>;