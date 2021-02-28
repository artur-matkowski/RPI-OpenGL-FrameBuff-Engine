#ifndef H_Transform3D
#define H_Transform3D
#include "ComponentBase.hpp"
#include "glm.hpp"

namespace asapi
{
	class Transform3D: public ComponentBase<Transform3D>
	{
	protected:
		SerializableArray<float>   	m_position;
		SerializableArray<float>   	m_rotation;
		SerializableArray<float>   	m_scale;

		glm::mat4 					m_modelMatix;
		glm::mat4 					m_MVP;
		
	public:
		Transform3D(bfu::MemBlockBase* mBlock);
		Transform3D(const Transform3D&) = delete;
		~Transform3D(){};


		void UpdateModelMatrix();
		//glm::mat4& GetModelMatrix() 		{ return m_modelMatix; } 
		glm::mat4& GetMVMatrix() 			{ return m_MVP; } 

		virtual void OnAttach() override;
		virtual void OnGUI() override;
	};
}

#endif