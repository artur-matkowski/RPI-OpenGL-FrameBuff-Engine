#include "UniformBase.hpp"
#include "MaterialType.hpp"
#include "Systems.hpp"
#include <GLES2/gl2.h>
#include <gtc/type_ptr.hpp>
#ifdef IS_EDITOR
#include "imgui.h"
#endif

namespace asapi
{

	UniformBase::~UniformBase()
	{}


	#ifdef IS_EDITOR
	void UniformBase::OnGUI()
	{
		ImGui::LabelText("Unsuported Uniform %s", "---");
	}
	#endif


	template<class T>
	void Uniform<T>::SetUniform(const T& in)
	{
		m_data = in;
		m_isDirty = true;
	}




	template class Uniform<ResourcePtr<Texture>>;
	template<>
	void Uniform<ResourcePtr< Texture >>::SendUniform()
	{
		m_data->BindTexture();
		//m_isDirty = false;
	}
	template<>
	void Uniform<ResourcePtr<Texture>>::SendUniform(const ResourcePtr<Texture>& override) const
	{
		override->BindTexture();
	}
	#ifdef IS_EDITOR
	template<>
	void Uniform<ResourcePtr<Texture>>::OnGUI()
	{
		ImGui::LabelText(m_name.c_str(), m_data->GetName());
		void* my_void_ptr = (void*)(intptr_t)m_data->GetTextureID();
		ImGui::Image(my_void_ptr, ImVec2(100.0f, 100.0f));
	}
	#endif

}