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
		glm::mat4 			m_projectionMatrix;
		glm::mat4* 			m_viewMatrix = nullptr;


		bfu::MemBlockBase*	p_metadataMemBlock;
		bfu::MemBlockBase*	p_materialsMemBlock;

	static void ResizeWidowCallback(void* _this, void* data);

	public:
		RendererSystem();
		~RendererSystem();

		void Init();
		void Render();


		static void ProcessMesh(Mesh*, void*);
		static void DispouseMesh(Mesh*);

		static Shader* ProcessShader(const char* vertex_source, const char* gragment_source, const char* shader_name);
		static void DispouseShader(Shader*);


		void RegisterRenderer(RendererComponent*);
		void UnRegisterRenderer(RendererComponent*);

		inline bfu::MemBlockBase* GetMetadataMemBlock(){ return p_metadataMemBlock; }
		inline bfu::MemBlockBase* GetMaterialsMemBlock(){ return p_materialsMemBlock; }

		void GetResolution(glm::ivec2& out){ out = m_resolution; }
		void UpdateProjectionMatrix(glm::mat4 projectionMatrix) { m_projectionMatrix = projectionMatrix; }
		void UpdateViewMatrix(glm::mat4* viewMatrix) { m_viewMatrix = viewMatrix; }
	};

}


#endif