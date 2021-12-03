#ifndef H_Texture2D
#define H_Texture2D
#include "ResourceBase.hpp"

namespace asapi
{
	class Texture2DHandleBase{};

	class Texture2D: public ResourceBase
	{
	protected:
		Texture2DHandleBase* 	m_textureHandle;

		virtual bool LoadResource() override;
		virtual void DispouseResource() override;

		Texture2D(uint64_t resourceID);
	public:
		Texture2D(const Texture2D& cp);
		~Texture2D();

	};
}

#endif