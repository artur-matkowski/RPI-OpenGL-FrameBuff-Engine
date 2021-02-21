#ifndef H_SerializableRenderer
#define H_SerializableRenderer 
#ifdef IS_EDITOR

namespace asapi
{
	class SerializableRendererBase
	{
	protected:
		char m_name[256];
	public:
		SerializableRendererBase(const char*);
		~SerializableRendererBase(){};

		virtual void OnGUI();
	};

	template <class T>
	class SerializableRenderer: public SerializableRendererBase
	{
	protected:
		T& m_obj;
	public:
		SerializableRenderer(T& ref, const char* name)
			:SerializableRendererBase(name)
			,m_obj(ref)
		{}

		~SerializableRenderer(){};


		virtual void OnGUI() override { this->SerializableRendererBase::OnGUI(); };
	};
}

#endif
#endif