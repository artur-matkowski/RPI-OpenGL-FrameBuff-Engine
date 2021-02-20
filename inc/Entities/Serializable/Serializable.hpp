#ifndef H_Serializable
#define H_Serializable
#include <bitforge/utils/bfu.hpp>

namespace asapi
{
	template<class T>
	class Serializable: public bfu::SerializableVar<T>
	{
	public:
		template<typename... Targs>
		Serializable(const char* Name, SerializableClassBase* parent, Targs... Fargs)
			:bfu::SerializableVar<T>(Name, parent, Fargs...)
		{
			
		}
	};
}

#endif