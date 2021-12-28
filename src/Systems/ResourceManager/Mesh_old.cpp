#include "Mesh_old.hpp"
#include <cstring>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Systems.hpp"

namespace asapi
{
	Mesh_old::Mesh_old(const char* path)
	{
        char buff[MAX_PATH_SIZE];
        sprintf(buff, "%s", path);

        SYSTEMS::IO::MMAP mmap;
        mmap.InitForRead(buff);

        if( mmap.IsValid() )
        {
            RendererSystem::ProcessMesh( this, mmap.Data() );
        }
	}

	Mesh_old::~Mesh_old()
	{
        RendererSystem::DispouseMesh(this);
	}



    void ProcessMesh(aiMesh *mesh)
    {
        const bool        m_hasPosition   = mesh->HasPositions();
        const bool        m_hasNormals    = false; //mesh->HasNormals();
        const uint32_t    m_numUvChannels = mesh->GetNumUVChannels();

        const uint32_t vertexfields = (m_hasPosition ? 3 : 0)
                                + (m_hasNormals ? 3 : 0)
                                + m_numUvChannels * 2;

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
            , SYSTEMS::GetObject().ASSETS.GetProjectPath()
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


        #ifndef IS_TARGET

        log::debug << "Mesh_old " << mesh->mName.C_Str() << " has " << *fp_arraySize << " floats:" << std::endl;
        for(int i=0; i<mesh->mNumVertices; ++i)
        {
            int index = i * vertexfields;
            std::cout << "vert[" << i << "] = ";

            std::cout << "( " <<vertexData[index];
            for(int j=1; j<vertexfields; ++j)
                std::cout << ", " << vertexData[index+j];
            std::cout << ")" << std::endl;
        }


        std::cout << "Indicies count: " << *fp_indiciesCount << " : \n";
        for(int i=0; i<*fp_indiciesCount; i+=3)
        {
            std::cout << "( " << indiciesData[i];
            std::cout << ", " << indiciesData[i+1];
            std::cout << ", " << indiciesData[i+2];
            std::cout << ")\n" ;
        }
        std::cout << "UVs: " << *fp_numUvChannels << " normals: " << *fp_hasNormals << std::endl;

        #endif

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
		const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate 
                                                    //| aiProcess_FlipUVs 
                                                    | aiProcess_FlipWindingOrder
                                                    | aiProcess_FindInvalidData
                                                    | aiProcess_ImproveCacheLocality
                                                    | aiProcess_FixInfacingNormals);	

		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
		{
			log::error << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}
		//directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}  

	void Mesh_old::Compile(const char* dest, const char* source)
	{
		loadModel(std::string(source));

        //mark asset as processed
        SYSTEMS::IO::MMAP mmap;
        mmap.InitForWrite(dest, 1);
	}
}