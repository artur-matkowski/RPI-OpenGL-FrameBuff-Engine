#ifndef H_Serializable
#define H_Serializable
#include "ComponentInterface.hpp"

namespace asapi
{
	template<class T>
	class Serializable: public bfu::SerializableVar<T>
	{
	public:
		template<typename... Targs>
		Serializable(const char* Name, ComponentInterface* parent, Targs... Fargs)
			:bfu::SerializableVar<T>(Name, parent, Fargs...)
		{
			parent->PushSerializableRenderer();
		}
	};
}

#endif