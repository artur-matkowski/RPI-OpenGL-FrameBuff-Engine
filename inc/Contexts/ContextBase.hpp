#ifndef _H_CONTEXTBASE
#define _H_CONTEXTBASE
#include <bitforge/utils/bfu.hpp>
#include "object.hpp"

namespace asapgl
{

	class ContextBase: public object
	{

	public:
		ContextBase(){};
		~ContextBase();

		virtual void SwapBuffer() = 0;
		virtual void GetResolution(int & width, int & height) = 0;
		virtual void HandleContextEvents() = 0;


	};

	
	class ResizeWindowArgs: public bfu::EventArgsBase
	{
	public:
		bfu::SerializableVar<int> m_width;
		bfu::SerializableVar<int> m_height;


	public:
		ResizeWindowArgs()
			:bfu::EventArgsBase()
			,m_width("m_width",this)
			,m_height("m_height",this)
		{
			m_width = -1;
			m_height = -1;
		}

		ResizeWindowArgs(const ResizeWindowArgs& copy)
			:bfu::EventArgsBase()
			,m_width("m_width",this)
			,m_height("m_height",this)
		{
			m_width = copy.m_width;
			m_height = copy.m_height;
		}
	};

}

#endif
