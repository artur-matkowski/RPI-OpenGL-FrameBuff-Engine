#ifndef H_Serializable
#define H_Serializable
#include "ComponentInterface.hpp"
#include "SerializableRenderer.hpp"

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
			#ifdef IS_EDITOR
			parent->PushSerializableRenderer( new SerializableRenderer< Serializable<T> >(*this, Name) );
			#endif
		}
	};

	template<class T>
	class SerializableArray: public bfu::SerializableVarArray<T>
	{
	public:
		template<typename... Targs>
		SerializableArray(const char* Name, ComponentInterface* parent, Targs... Fargs)
			:bfu::SerializableVarArray<T>(Name, parent, Fargs...)
		{
			#ifdef IS_EDITOR
			parent->PushSerializableRenderer( new SerializableRenderer< SerializableArray<T> >(*this, Name) );
			#endif
		}
	};
}

#endif