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
		static GLfloat vertexbuff[] = {
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f,// 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,// 0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f//, 0.0f, 0.0f, 1.0f, 1.0f
		};
		//	^ verts 			^ UVs 		^ colors

		static GLuint indices[3] = {0, 1, 2};
		m_size = 3;


 		glGenBuffers(1, &vertex_buffer);
 		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_size * 5, 0, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * m_size * 5, vertexbuff);


        glGenBuffers(1, &indice_array);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indice_array);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*m_size, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*m_size, indices);

        config.push_back( vertex_buffer );
        config.push_back( indice_array );
        config.push_back( 2 ); //present attributes count

        //positions attribute
        config.push_back(0);
        config.push_back(0);
        config.push_back(3);
        config.push_back(5);
        config.push_back(0);

        //uv0 attribute
        config.push_back(2);
        config.push_back(2);
        config.push_back(2);
        config.push_back(5);
        config.push_back(3);

        config.push_back( m_size );

/*
        char buff[MAX_PATH_SIZE];
        sprintf(buff, "%s.mmp", path);

        SYSTEMS::IO::MMAP mmap;
        mmap.InitForRead(buff);

        if( !mmap.IsValid() )
            return;

        fp_hasPosition = (bool*) (size_t)mmap.Data();
        fp_hasNormals = &fp_hasPosition[1];
        fp_arraySize = (uint32_t*) &fp_hasPosition[2];
        fp_numUvChannels = &fp_arraySize[1];
        fp_indiciesCount = &fp_numUvChannels[1];
        fp_vertexData = (float*) &fp_indiciesCount[1];
        fp_indiciesData = (int*) &fp_vertexData[*fp_arraySize];

        const uint32_t vertexfields = (*fp_hasPosition ? 3 : 0)
                                + (*fp_hasNormals ? 3 : 0)
                                + *fp_numUvChannels * 2;


        if(fp_hasPosition)
        {
            config.reserve(config.size()+5);
            config.push_back(0);
            config.push_back(0);
            config.push_back(3);
            config.push_back(vertexfields);
            config.push_back(0);
            //glEnableVertexAttribArray(0);
            //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*9, nullptr);
        }

        // if(m_hasNormals)
        // {
        //     config.reserve(config.size()+5);
        //     config.push_back(0);
        //     config.push_back(0);
        //     config.push_back(3);
        //     config.push_back(9);
        //     config.push_back(0);
        // }

        for(uint32_t UVchannel = 0; UVchannel<*fp_numUvChannels; ++UVchannel)
        {
            config.reserve(config.size()+5);
            config.push_back(2);
            config.push_back(2);
            config.push_back(2);
            config.push_back(vertexfields);
            config.push_back(3);
            // glEnableVertexAttribArray(2);
            // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*9, (void*) (sizeof(GL_FLOAT)*3) );
        }


        log::debug << "Mesh " << path << " has " << *fp_arraySize << " floats:" << std::endl;
        for(int i=0; i<(*fp_arraySize/vertexfields); ++i)
        {
            int index = i * vertexfields;
            std::cout << "vert[" << i << "] = ";

            std::cout << "( " <<fp_vertexData[index];
            for(int j=1; j<vertexfields; ++j)
                std::cout << ", " << fp_vertexData[index+j];
            std::cout << ")" << std::endl;
        }
        std::cout << "Indicies count: " << *fp_indiciesCount << " : \n";
        for(int i=0; i<*fp_indiciesCount; i+=3)
        {
            std::cout << "( " << fp_indiciesData[i];
            std::cout << ", " << fp_indiciesData[i+1];
            std::cout << ", " << fp_indiciesData[i+2];
            std::cout << ")\n" ;
        }
        std::cout << "UVs: " << *fp_numUvChannels << " normals: " << *fp_hasNormals << std::endl;

*/
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

        //allocte memory maped file to convert the data
        SYSTEMS::IO::MMAP mmap;
        mmap.InitForWrite(buff,
                sizeof(bool) * 2
                + sizeof(uint32_t) * 3
                + sizeof(float) * arraySize
                + sizeof(int) * indiciesCount);

        bool* fp_hasPosition = (bool*) (size_t)mmap.Data();
        bool* fp_hasNormals = &fp_hasPosition[1];
        uint32_t* fp_arraySize = (uint32_t*) &fp_hasPosition[2];
        uint32_t* fp_numUvChannels = &fp_arraySize[1];
        uint32_t* fp_indiciesCount = &fp_numUvChannels[1];
        vertexData = (float*) &fp_indiciesCount[1];
        indiciesData = (int*) &vertexData[arraySize];

        *fp_hasPosition = m_hasPosition;
        *fp_hasNormals = m_hasNormals;
        *fp_arraySize = arraySize;
        *fp_numUvChannels = m_numUvChannels;
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


        ///////////////////
        //indicies

        uint32_t i = 0;
        for(uint32_t f = 0; f < mesh->mNumFaces; ++f)
        {
            aiFace face = mesh->mFaces[f];
            // retrieve all indices of the face and store them in the indices array
            for(unsigned int j = 0; j < face.mNumIndices; ++j){
                indiciesData[i] = face.mIndices[j]; ++i;
            }
        }


        log::debug << "Mesh " << mesh->mName.C_Str() << " has " << *fp_arraySize << " floats:" << std::endl;
        for(int i=0; i<mesh->mNumVertices; ++i)
        {
            int index = i * vertexfields;
            std::cout << "vert[" << i << "] = ";

            std::cout << "( " <<vertexData[index];
            for(int j=1; j<vertexfields; ++j)
                std::cout << ", " << vertexData[index+j];
            std::cout << ")" << std::endl;
        }


        // log::debug << "Mesh " << mesh->mName.C_Str() << " has " << *fp_arraySize << " floats:" << std::endl;
        // for(int i=0; i<mesh->mNumVertices; ++i)
        // {
        //     int index = i * vertexfields;
        //     std::cout << "vert[" << i << "] = ";

        //     std::cout << "( " << mesh->mVertices[i].x;
        //     std::cout << ", " << mesh->mVertices[i].y;
        //     std::cout << ", " << mesh->mVertices[i].z;
        //     std::cout << ", " << mesh->mNormals[i].x;
        //     std::cout << ", " << mesh->mNormals[i].y;
        //     std::cout << ", " << mesh->mNormals[i].z;
        //     std::cout << ", " << mesh->mTextureCoords[0][i].x;
        //     std::cout << ", " << mesh->mTextureCoords[0][i].y;

        //     std::cout << ")" << std::endl;
        // }


        std::cout << "Indicies count: " << *fp_indiciesCount << " : \n";
        for(int i=0; i<*fp_indiciesCount; i+=3)
        {
            std::cout << "( " << indiciesData[i];
            std::cout << ", " << indiciesData[i+1];
            std::cout << ", " << indiciesData[i+2];
            std::cout << ")\n" ;
        }
        std::cout << "UVs: " << *fp_numUvChannels << " normals: " << *fp_hasNormals << std::endl;



    }




    void processNode(aiNode *node, const aiScene *scene)
    {
        // process each mesh located at the current node
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
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