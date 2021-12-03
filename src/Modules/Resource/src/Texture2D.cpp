#include "Texture2D.hpp"

namespace asapi
{
	Texture2D::Texture2D(uint64_t resourceID)
		:ResourceBase( resourceID )
	{

	}
	Texture2D::Texture2D(const Texture2D& cp)
		:ResourceBase( cp )
	{

	}

	Texture2D::~Texture2D()
	{

	}

	bool Texture2D::LoadResource()
	{
		return false;
	}
	void Texture2D::DispouseResource()
	{

	}
}
