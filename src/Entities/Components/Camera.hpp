#ifndef _H_CAMERA
#define _H_CAMERA
#include "ComponentBase.hpp"
#include "glm.hpp"


namespace asapi
{
	class Camera: public ComponentBase<Camera>
	{	
	protected:
		SERIALIZABLE_VAR( Camera, float, m_nearPlane );
		SERIALIZABLE_VAR( Camera, float, m_farPlane );
		SERIALIZABLE_VAR( Camera, float, m_FOV );
		SERIALIZABLE_VAR( Camera, bool, m_isMainCamera );


		glm::mat4 					m_projectionMatrix;
	public:
		Camera(bfu::MemBlockBase* mBlock);


		// virtual void OnAttach() override;
		virtual void OnIsDirty() override;

		#ifdef IS_EDITOR
		virtual void OnGUI() override;
		#endif

		bool IsMainCamera()				{ return m_isMainCamera; }
		void SetMainCamera(bool state)	{ m_isMainCamera = state; }
	};
}

#endif