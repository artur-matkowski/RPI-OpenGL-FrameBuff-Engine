#ifndef _H_RENDERERSYSTEM
#define _H_RENDERERSYSTEM
#include "object.hpp"



#define ATTR_LOC_position 	0
#define ATTR_LOC_normal 	1
#define ATTR_LOC_uv0 		2
#define ATTR_LOC_uv1 		3
#define ATTR_LOC_uv2 		4
#define ATTR_LOC_uv3 		5
#define ATTR_LOC_color0		6
#define ATTR_LOC_color1		7
#define ATTR_LOC_color2		8
#define ATTR_LOC_color3		9



namespace asapi{

	class RendererComponent;

	struct _RendererHandle{};
	typedef _RendererHandle* tRendererHandle;

	struct _MeshHandle: public _RendererHandle{};
	typedef _MeshHandle* tMeshHandle;

	struct _ShaderHandle: public _RendererHandle{};
	typedef _ShaderHandle* tShaderHandle;

	class Mesh_old;
	class Shader;

	class RendererSystem: public object
	{
		glm::ivec2			m_resolution;
		glm::mat4 			m_projectionMatrix;
		glm::mat4* 			m_viewMatrix = nullptr;

		bfu::MemBlockBase*	p_materialsMemBlock;

	static void ResizeWidowCallback(void* _this, void* data);

	public:
		RendererSystem();
		~RendererSystem();

		void Init();
		void Render();


		static void ProcessMesh(Mesh_old*, void*);
		static void DispouseMesh(Mesh_old*);

		static Shader* ProcessShader_obsolete(const char* vertex_source, const char* gragment_source, const char* shader_name, Shader* out = nullptr);
		static void DispouseShader_obsolete(Shader*);


		void RegisterRenderer(RendererComponent*);
		void UnRegisterRenderer(RendererComponent*);

		inline bfu::MemBlockBase* GetMaterialsMemBlock(){ return p_materialsMemBlock; }

		void GetResolution(glm::ivec2& out){ out = m_resolution; }
		void UpdateProjectionMatrix(glm::mat4 projectionMatrix) { m_projectionMatrix = projectionMatrix; }
		void UpdateViewMatrix(glm::mat4* viewMatrix) { m_viewMatrix = viewMatrix; }


		void * debugComponent = 0;
	};

}


#endif