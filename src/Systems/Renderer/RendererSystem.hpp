#ifndef _H_RENDERERSYSTEM
#define _H_RENDERERSYSTEM
#include "object.hpp"



namespace asapi{

	class RendererComponent;

	struct _RendererHandle{};
	typedef _RendererHandle* tRendererHandle;

	struct _MeshHandle: public _RendererHandle{};
	typedef _MeshHandle* tMeshHandle;

	struct _ShaderHandle: public _RendererHandle{};
	typedef _ShaderHandle* tShaderHandle;

	class Mesh;
	class Shader;

	class RendererSystem: public object
	{
		glm::ivec2			m_resolution;


		//RendererComponent* 	m_rendererComponent = 0;

	static void ResizeWidowCallback(void* _this, void* data);

	public:
		RendererSystem();
		~RendererSystem();

		void Init();
		void Render();


		static bool ProcessMesh(Mesh*);
		static bool DispouseMesh(Mesh*);

		static bool ProcessShader(Shader*);
		static bool DispouseShader(Shader*);


		void RegisterRenderer(RendererComponent*);
		void UnRegisterRenderer(RendererComponent*);
	};

}


#endif