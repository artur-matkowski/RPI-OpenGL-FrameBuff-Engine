#ifndef _H_UniformTexture
#define _H_UniformTexture
#include "UniformBase.hpp"
#include "ResourcePtr.hpp"
#include "Texture.hpp"

namespace asapgl
{
	template<>
	class Uniform< ResourcePtr< Texture > >: public UniformBase
	{
		bfu::SerializableVar<std::string>	m_data;
		ResourcePtr< Texture >				m_texture;

	public:
		Uniform(uint32_t location, MaterialType* owner)
			:UniformBase(location, owner)
			,m_data("m_data", this)
		{};
		~Uniform();
	};
}

#endif