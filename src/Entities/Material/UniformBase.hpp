#ifndef _H_UniformBase
#define _H_UniformBase
#include "object.hpp"
#include <bitforge/utils/bfu.hpp>

namespace asapi
{
	class MaterialType;
	class MaterialInstance;

	using bfu::stream;

	class UniformInterface: public object
	{
	protected:
		uint32_t 				m_location = -1;
		bool 					m_isDirty = false;
	public:
		UniformInterface(uint32_t location, bfu::MemBlockBase* metadataMemBlock)
			:m_location(location)
		{
		};
		~UniformInterface();

		virtual void SendUniform() = 0;
		#ifdef IS_EDITOR
		virtual void OnGUI();
		#endif

		inline void SetUniformLocation(const uint32_t& location)
		{
			m_location = location;
		}
		virtual bool Is(const char* name) = 0;
	};
	
}

#endif