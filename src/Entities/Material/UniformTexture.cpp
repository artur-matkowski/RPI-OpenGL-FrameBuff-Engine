#include "UniformTexture.hpp"
#include <GLES2/gl2.h>
#ifdef IS_EDITOR
#include "imgui.h"
#endif

namespace asapi
{

	void UniformTexture::SetUniform(const ResourcePtr<Texture>& in)
	{
		m_data = in;
		m_isDirty = true;
	}

	void UniformTexture::SendUniform()
	{
		m_data->BindTexture();
	}
	void UniformTexture::SendUniform(const ResourcePtr<Texture>& override) const
	{
		override->BindTexture();
	}
	#ifdef IS_EDITOR
	void UniformTexture::OnGUI()
	{
		ImGui::LabelText(m_name.c_str(), m_data->GetName());
		void* my_void_ptr = (void*)(intptr_t)m_data->GetTextureID();
		ImGui::Image(my_void_ptr, ImVec2(100.0f, 100.0f));
	}
	#endif

}