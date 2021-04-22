#include "Mesh.hpp"
#include <cstring>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Systems.hpp"

namespace asapi
{
	Mesh::Mesh(const char* path)
	{
		#ifdef IS_EDITOR
		int size = strlen(path);
		const char* tmp = &path[size-1];
		for(; *tmp!='/'; tmp-=1); // < find where file name is starting
		strncpy(name, tmp+1, 255);
		name[255] = '\0';
		#endif

		static GLfloat vertexbuff[] = {
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};
		//	^ verts 			^ UVs 		^ colors

		static GLuint indices[3] = {0, 1, 2};
		m_size = 3;


 		glGenBuffers(1, &vertex_buffer);
 		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_size * 9, 0, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * m_size * 9, vertexbuff);


        glGenBuffers(1, &indice_array);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indice_array);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*m_size, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*m_size, indices);

	}


	Mesh::Mesh(glm::vec2 resolution)
	{
		static GLfloat vertexbuff[] = {
			 0.5f, -0.1f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.1f, -0.5f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			 1.0f, -0.9f, 0.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.9f, -1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f
		};
		static GLfloat vertexbuff0[] = {
			 0.5f, -0.1f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.1f, -0.5f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			 1.0f, -0.9f, 0.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.9f, -1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f
		};
		//	^ verts 			^ UVs 		^ colors

		static GLuint indices[6] = {0, 1, 2, 4, 5, 6};

		m_size = 6;

		for(int i=0; i<9*6; i+=9)
		{
			vertexbuff[i] = vertexbuff0[i] * resolution.y/resolution.x;
			vertexbuff[i+1] = vertexbuff0[i+1] * resolution.y/resolution.x;
		}

		for(int i=0; i<9*6; i+=9)
		{
			vertexbuff[i] = vertexbuff[i] * 50.f/resolution.x;
			vertexbuff[i+1] = vertexbuff[i+1] * 50.f/resolution.y;
		}


 		glGenBuffers(1, &vertex_buffer);
 		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_size * 9, 0, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * m_size * 9, vertexbuff);


        glGenBuffers(1, &indice_array);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indice_array);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*m_size, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*m_size, indices);
	}
	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &vertex_buffer);
		glDeleteBuffers(1, &indice_array);
	}

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};
using std::vector;

    void ProcessMesh(aiMesh *mesh)
    {
        const bool        m_hasPosition   = mesh->HasPositions();
        const bool        m_hasNormals    = mesh->HasNormals();
        const uint32_t    m_numUvChannels = mesh->GetNumUVChannels();

        const uint32_t vertexfields = (m_hasPosition ? 3 : 0)
                                + (m_hasNormals ? 3 : 0)
                                + m_numUvChannels * 2;

        //const uint32_t vertexMemSize = vertexfields * sizeof(float);

        const uint32_t arraySize = vertexfields * mesh->mNumVertices;

        int vertexInnerDataFieldOffset = 0;
        int VBOconfigInnerDataFieldOffset = 0;



        float* vertexData = nullptr;
        int* indiciesData = nullptr;


        uint32_t indiciesCount = 0;
        for(uint32_t i = 0; i < mesh->mNumFaces; ++i)
        {
            aiFace face = mesh->mFaces[i];
            indiciesCount += face.mNumIndices;      
        }

        char buff[1024];
        sprintf(buff, "%s/assets_int/meshes/%s.mmp"
            , SYSTEMS::GetObject().RESOURCES.GetProjectPath()
            , mesh->mName.C_Str());

        SYSTEMS::IO::MMAP mmap;
        mmap.InitForWrite(buff,
                sizeof(uint32_t) * 2
                + sizeof(float) * arraySize
                + sizeof(int) * indiciesCount);
        int* fp_arraySize = (int*) (size_t)mmap.Data();
        int* fp_indiciesCount = &fp_arraySize[1];
        vertexData = (float*)((size_t)mmap.Data() + sizeof(int)*2);
        indiciesData = (int*)((size_t)mmap.Data() + sizeof(int)*2
                                        + sizeof(float) * arraySize);

        *fp_arraySize = arraySize;
        *fp_indiciesCount = indiciesCount;


        ////////////////////////
        //vertecies
        if(m_hasPosition)
        {
            for(uint32_t i = 0; i<mesh->mNumVertices; ++i)
            {
                int index = i * vertexfields + vertexInnerDataFieldOffset;
                vertexData[index + 0] = mesh->mVertices[i].x;
                vertexData[index + 1] = mesh->mVertices[i].y;
                vertexData[index + 2] = mesh->mVertices[i].z;
            }
            vertexInnerDataFieldOffset += 3;
        }

        if(m_hasNormals)
        {
            for(uint32_t i = 0; i<mesh->mNumVertices; ++i)
            {
                int index = i * vertexfields + vertexInnerDataFieldOffset;
                vertexData[index + 0] = mesh->mNormals[i].x;
                vertexData[index + 1] = mesh->mNormals[i].y;
                vertexData[index + 2] = mesh->mNormals[i].z;
            }
            vertexInnerDataFieldOffset += 3;
        }

        for(uint32_t UVchannel = 0; UVchannel<m_numUvChannels; ++UVchannel)
        {
            for(uint32_t i = 0; i<mesh->mNumVertices; ++i)
            {
                int index = i * vertexfields + vertexInnerDataFieldOffset;
                vertexData[index + 0] = mesh->mTextureCoords[UVchannel][i].x; 
                vertexData[index + 1] = mesh->mTextureCoords[UVchannel][i].y; 
            }
            vertexInnerDataFieldOffset += 2;
        }

        log::debug << "Mesh " << mesh->mName.C_Str() << " has " << mesh->mNumVertices << " vertecies" << std::endl;
        for(int i=0; i<mesh->mNumVertices; ++i)
        {
            int index = i * vertexfields;
            log::debug << "vert[" << i << "] = ("
                << vertexData[index+0] << ", " << vertexData[index+1] << ", " << vertexData[index+2] << std::endl;
                //<< vertexData[i] << ", " << vertexData[i] << ")" << std::endl;
        }




        ///////////////////
        //indicies

        uint32_t i = 0;
        for(uint32_t f = 0; f < mesh->mNumFaces; ++f)
        {
            aiFace face = mesh->mFaces[f];
            // retrieve all indices of the face and store them in the indices array
            for(unsigned int j = 0; j < face.mNumIndices; ++j)
                indiciesData[i] = face.mIndices[j]; 
        }
    }



	void processMesh(aiMesh *mesh, const aiScene *scene)
    {
        // data to fill
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        //vector<Texture> textures;

        // walk through each of the mesh's vertices
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x; 
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);        
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN
/*
        // 1. diffuse maps
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
*/
        log::debug << "Mesh " << mesh->mName.C_Str() << " has " << (int)vertices.size() << " vertecies" << std::endl;
        for(int i=0; i<vertices.size(); ++i)
        {
        	log::debug << "vert[" << i << "] = ("
        		<< vertices[i].Position.x << ", " << vertices[i].Position.y << ", " << vertices[i].Position.z << std::endl;
        		//<< vertices[i].TexCoords.x << ", " << vertices[i].TexCoords.y << ")" << std::endl;
        }
        
    }

    void processNode(aiNode *node, const aiScene *scene)
    {
        // process each mesh located at the current node
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            //meshes.push_back(processMesh(mesh, scene));
           //processMesh(mesh, scene);
           ProcessMesh(mesh);
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }

    }

	void loadModel(std::string path)
	{
		Assimp::Importer import;
		const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);	

		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
		{
			log::error << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}
		//directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}  

	void Mesh::Compile(const char* dest, const char* source)
	{
		loadModel(std::string(source));

        //mark asset as processed
        SYSTEMS::IO::MMAP mmap;
        mmap.InitForWrite(dest, 1);
	}
}