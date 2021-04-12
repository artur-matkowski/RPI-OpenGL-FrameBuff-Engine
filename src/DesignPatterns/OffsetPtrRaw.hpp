#ifndef _H_OffsetPtr
#define _H_OffsetPtr

template<class T>
class OffsetPtrRaw
{
	T* 		p_data = 0;
	size_t 	m_offset = 0;
public:
	OffsetPtrRaw(void* origin, T* target)
		:p_data(target)
		,p_offset((size_t)target - (size_t)origin)
	{
	}


	template<class U>
	OffsetPtrRaw(const OffsetPtrRaw<U> cp)
		:p_data((T*)cp.p_data)
		,p_offset(cp.p_offset)
	{
	}

	~OffsetPtrRaw(){};

	void Realign_NotRaw(void* origin)
	{
		if(m_offset!=0) // check if ptr is in raw state
			p_data = (T*)((size_t)origin + m_offset);
	}

	void Realign_DetectMoved_NotRaw(void* oldOrigin, void* newOrigin)
	{
		void* origin = (void*) ((size_t)p_data-m_offset);

		if(origin != oldOrigin) // check if my origin is the one that moved
			return;

		if(m_offset!=0) // check if ptr isn't in raw state
			p_data = (T*)((size_t)origin + m_offset);
	}

    T& operator* ()
	{
	    return *p_data;
	}

    T* operator-> ()
	{
	    return p_data;
	}

	template<class U>
	T& operator=(const OffsetPtrRaw<U> cp)
	{
		p_data = (T*)cp.p_data;
		p_offset = cp.p_offset;
	}
}

#endif