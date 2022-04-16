#include "MaterialInstance.hpp"
#include "ResourcePtr.hpp"
#include "Texture.hpp"
#include "imgui.h"

namespace asapi
{
	std::string MaterialInstance::s_projectPath;

	MaterialInstance::MaterialInstance(const UniqueID& uuid)
	{
		FILE::MMAP materialDataFile;
		bfu::JSONSerializer serializer;

		m_uuid = uuid;
		m_shaderResource.BindOnDirtyCallback(OnShaderDirtyCallback, this);

		std::string materialDataPath = s_projectPath + RESOURCE_BINARIES_DIR "/";
		materialDataPath += std::to_string( uuid.ID() ) + MATERIAL_DATA_EXTENSION;


		materialDataFile.InitForRead( materialDataPath.c_str() );

		log::debug << "Opening MaterialInstance (data) " << materialDataPath.c_str() << std::endl;

		serializer.assignData( (char*)materialDataFile.Data(), materialDataFile.Size() );
		serializer.Deserialize( this );
	}

	MaterialInstance::~MaterialInstance()
	{}
	
	void MaterialInstance::SetProjectPath(const std::string& path)
	{
		s_projectPath = path;
	}
	
	void MaterialInstance::OnShaderDirtyCallback(void* data)
	{
		MaterialInstance* _this = (MaterialInstance*)data;

		asapi::FILE::MMAP* file = (asapi::FILE::MMAP*) _this->m_shaderResource.GetRawHandle();
			
		uint16_t* p_vertSize = (uint16_t*)file->Data();
		uint16_t* p_fragSize = &p_vertSize[1];
		uint16_t* p_shaderNameSize = &p_vertSize[2];
		char* vertex_source = (char*)&p_vertSize[3];
		char* fragment_source = vertex_source+*p_vertSize+1;
		char* p_nameBuff = fragment_source+*p_fragSize+1;



		_this->m_shader.LoadShaderFromResourceRawHandle( _this->m_shaderResource.GetRawHandle() );

		_this->m_shader.UseProgram();


		GLint i;
		GLint count;
		uint32_t location;

		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		const GLsizei bufSize = 128; // maximum name length
		GLchar name[bufSize] = {0}; // variable name in GLSL
		GLsizei length; // name length	
		int32_t newUniformsCount = 0;


		glGetProgramiv(_this->m_shader.GetProgramID(), GL_ACTIVE_UNIFORMS, &newUniformsCount);
		printf("Active Uniforms: %d\n", newUniformsCount);


		for(int i=0; i<_this->m_uniformsCount; ++i)
		{
			delete _this->p_uniforms[i];
		}

		if( newUniformsCount > _this->m_uniformsCount )
		{
			if( _this->p_uniforms!=nullptr )
				delete _this->p_uniforms;

			//_this->p_uniforms = (UniformInterface**)materialsMemBlock->allocate(newUniformsCount, sizeof(UniformInterface*), alignof(UniformInterface*));
			_this->p_uniforms = new (UniformInterface*);
		}
		_this->m_uniformsCount = newUniformsCount;

		

		for (i = 0; i < _this->m_uniformsCount; ++i)
		{
		    glGetActiveUniform(_this->m_shader.GetProgramID(), (GLuint)i, bufSize, &length, &size, &type, name);

		    printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
		    location = glGetUniformLocation(_this->m_shader.GetProgramID(), name);

		    //TODO custom_allocator instead of operator new
		    switch(type)
		    {
		    	case GL_FLOAT:
		    		//_this->p_uniforms[i] = (UniformInterface*)materialsMemBlock->allocate(1, sizeof(Uniform<float>), alignof(Uniform<float>));
		    		_this->p_uniforms[i] = new Uniform<float>(location, name);
		    		break;
		    	case GL_FLOAT_VEC3:
		    		//_this->p_uniforms[i] = (UniformInterface*)materialsMemBlock->allocate(1, sizeof(Uniform<glm::vec3>), alignof(Uniform<glm::vec3>));
		    		_this->p_uniforms[i] = new  Uniform<glm::vec3>(location, name);
		    		break;
		    	case GL_FLOAT_VEC4:
		    		//_this->p_uniforms[i] = (UniformInterface*)materialsMemBlock->allocate(1, sizeof(Uniform<glm::vec4>), alignof(Uniform<glm::vec4>));
		    		_this->p_uniforms[i] = new Uniform<glm::vec4>(location, name);
		    		break;
		    	case GL_FLOAT_MAT4:
		    		//_this->p_uniforms[i] = (UniformInterface*)materialsMemBlock->allocate(1, sizeof(Uniform<glm::mat4>), alignof(Uniform<glm::mat4>));
		    		_this->p_uniforms[i] = new Uniform<glm::mat4>(location, name);
		    		break;
		    	case GL_SAMPLER_2D:
		    		//_this->p_uniforms[i] = (UniformInterface*)materialsMemBlock->allocate(1, sizeof(Uniform<ResourcePtr<Texture>>), alignof(Uniform<ResourcePtr<Texture>>));
		    		_this->p_uniforms[i] = new Uniform<ResourcePtr<Texture>>(location, name);
		    		break;		    		
		    	default:
		    		char buff[128];
		    		sprintf(buff, "%#04X", type);
		    		log::warning << "Unsuported uniform type found in " << p_nameBuff << ": " << name << " type: " << buff << std::endl;
		    		break;
		    }
		}

		_this->p_modelViewUniform = (Uniform<glm::mat4>*) _this->GetUniformPtr("modelViewMat");


		log::debug << "MaterialInstance::OnShaderDirtyCallback" << std::endl;
	}


	void MaterialInstance::PreSerializationCallback()
	{
		for( int i = m_uniformsData.size(); i<m_uniformsCount; ++i )
		{
			m_uniformsData.push_back( new UniformInfo() );
		}
		while( m_uniformsData.size() > m_uniformsCount )
		{
			delete m_uniformsData.back();
			m_uniformsData.erase( m_uniformsData.end()-- );
		}

		char buff[1024];

		for(int16_t i=0; i<m_uniformsCount; ++i)
		{
			m_uniformsData[i].m_uniformName = p_uniforms[i]->GetName();
			p_uniforms[i]->sprintf( buff );
			m_uniformsData[i].m_recreationString = buff;
		}
	}
	void MaterialInstance::PostDeserializationCallback()
	{
		for(int i=0; i<m_uniformsData.size(); ++i)
		{
			#ifdef IS_EDITOR
			if( GetUniformPtr( m_uniformsData[i].m_uniformName.c_str() ) == nullptr )
			{
				log::error << "Could not find uniform " << m_uniformsData[i].m_uniformName.c_str() << std::endl;
				continue;
			}
			#endif

			GetUniformPtr( m_uniformsData[i].m_uniformName.c_str() )->sscanf( m_uniformsData[i].m_recreationString.c_str() );
		}
	}

	#ifdef IS_EDITOR
	void MaterialInstance::OnGUI_SelectShader()
	{
		char buff[256];
		snprintf(buff, 256, "Shader resource##%llu", m_uuid.ID());
		if (ImGui::BeginCombo(buff, "m_shaderName"))
        {
            m_shaderResource.OnGUI_caller();
            ImGui::EndCombo();
        }
	}
	void MaterialInstance::OnGUI()
	{
		const ImVec4 warningColor(1.0, 1.0, 0.0, 1.0);

		if( m_shader.IsValid() )
		{
			ImGui::Text("Material Instance is valid.");

			for(int16_t i=0; i<m_uniformsCount; ++i)
			{
				const bool uniformUpdated = p_uniforms[i]->OnGUI();
				m_uniformsChanged = m_uniformsChanged || uniformUpdated;
			}

			if( m_uniformsChanged )
			{
				ImGui::TextColored(warningColor, "Material Uniforms not serialized.");
			}
			if( ImGui::Button("Save Material") )
			{
				m_uniformsChanged = false;

				SerializeMaterial();
			}
		}
		else
		{
			ImGui::TextColored(warningColor, "Material Instance is not valid.");
		}
	}

	void MaterialInstance::SerializeMaterial()
	{
		FILE::STREAM materialDataFile;
		bfu::JSONSerializer serializer;


		serializer.Serialize( this );

		std::string materialDataPath = s_projectPath + RESOURCE_BINARIES_DIR "/";
		materialDataPath += std::to_string( m_uuid.ID() ) + MATERIAL_DATA_EXTENSION;


		materialDataFile.InitForWrite( materialDataPath.c_str() );
		materialDataFile.Write( serializer.data(), serializer.size() );
	}
	#endif
}