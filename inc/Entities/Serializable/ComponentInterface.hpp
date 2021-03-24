#ifndef H_ComponentInterface
#define H_ComponentInterface
#include "EntityBase.hpp"

namespace asapi
{
	class GameObject;
	class ComponentInterface;

	typedef ComponentInterface* (*InitFuncPtr)(bfu::MemBlockBase*);

	#define TYPE_INFO_CAPACITY 1024
	struct TypeInfo
	{
		InitFuncPtr 		fPtr;
		size_t 				id;
		size_t 				sizeOf;
		const char* 		name;

		static void RegisterType(InitFuncPtr, size_t, size_t, const char*);
		static TypeInfo* GetTypeInfo(size_t);
		static TypeInfo* GetTypeInfo(const char*);
		static TypeInfo* GetTypeInfo();
		static int GetTypeInfoSize();
	};


	class ComponentInterface: public object
	{
	protected:
		GameObject *m_owner = nullptr;
	public:	
		ComponentInterface(bfu::MemBlockBase* mBlock);
		~ComponentInterface(){};
		
		void Attached(GameObject* owner);
		void Detached();

		static ComponentInterface* AllocateAndInitObjectFromTypeHash(size_t hash, bfu::MemBlockBase* mBlock)
		{
			return TypeInfo::GetTypeInfo(hash)->fPtr(mBlock);
		}

		#ifdef IS_EDITOR
		void OnGUI_NameAndVirtual();
		virtual void OnGUI();
		#endif


		virtual void OnAttach(){};
		virtual void OnDetach(){};
		virtual void OnIsDirty(){};
		virtual size_t TypeHash() = 0;

		virtual const char* TypeName()
		{
			return "ComponentInterface";
		}

		static void RemovedMarkedComponent();
	};
}


#endif