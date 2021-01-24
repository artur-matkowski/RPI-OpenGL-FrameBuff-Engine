#include "GameObject.hpp"

namespace asapgl
{

	#ifndef _PLAYER
		void GameObject::Serialize(bfu::JSONStream& stream)
		{
			//sync objects to serializable IDs
			//bfu::SerializableClassBase::Serialize(stream);
		}
		void GameObject::Deserialize(bfu::JSONStream& stream)
		{
			//bfu::SerializableClassBase::Deserialize(stream);
			//sync objects from serializable IDs
		}
	#endif

	void GameObject::Serialize(std::ofstream& stream)
	{

	}
	void GameObject::Deserialize(std::ifstream& stream)
	{

	}
}