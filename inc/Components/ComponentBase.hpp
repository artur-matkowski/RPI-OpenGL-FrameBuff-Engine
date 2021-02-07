#ifndef _H_ComponentBase
#define _H_ComponentBase
#include "EntityBase.hpp"


namespace asapgl
{
	class ComponentInterface: public EntityBase
	{
	public:	
		ComponentInterface(bfu::MemBlockBase* mBlock)
			:EntityBase(mBlock)
		{};
		~ComponentInterface(){};
		
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;

		//suve component type
		//virtual void Serialize(bfu::JSONStream& stream);

		//resotre component type
		//virtual void Deserialize(bfu::JSONStream& stream);
	};

	template<class T>
	class ComponentBase: public ComponentInterface
	{
	protected:
	public:	
		ComponentBase(bfu::MemBlockBase* mBlock)
			:ComponentInterface(mBlock)
		{};
		~ComponentBase(){};

		static T* AllocateAndInit( bfu::MemBlockBase* mBlock )
		{
			T* obj = mBlock->allocate(1, sizeof(T), alignof(T) );
			obj->Init(mBlock);
			return obj;
		}
	};
}

#endif