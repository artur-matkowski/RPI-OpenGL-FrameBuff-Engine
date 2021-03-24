#ifndef H_Serializable
#define H_Serializable
#include "ComponentInterface.hpp"
#include "SerializableRenderer.hpp"

namespace asapi
{
	/*
	#ifdef IS_EDITOR
	bfu::MemBlockBase* GetGUIAllocator();
	#endif

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
			SerializableRenderer< SerializableArray<T> >* ptr
				 = (SerializableRenderer< SerializableArray<T> >*)GetGUIAllocator()->allocate(1
				 			,sizeof(SerializableRenderer< SerializableArray<T> >)
				 			,alignof(SerializableRenderer< SerializableArray<T> >));
			new (ptr) SerializableRenderer< SerializableArray<T> >(*this, Name);
			parent->PushSerializableRenderer( ptr );
			#endif
		}
	};*/
}

#endif