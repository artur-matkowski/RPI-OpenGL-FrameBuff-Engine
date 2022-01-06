#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "ResourceFBXProcessor.hpp"
#include "ResourceSystem.hpp"
#include "RendererSystem.hpp"
#include "Systems.hpp"
#include "bfu.hpp"

namespace asapi
{	
	void loadModel(std::string path
					, const asapi::ResourceTracker& in_currentResource
					, asapi::FILE::MMAP* in_resourceFile
					, const char* in_projectPath
					, std::vector<asapi::SubResourceData>* out_resourceBinaries);



	void* ResourceFBXProcessor::LoadResource(const char* path)
	{
		asapi::FILE::MMAP file;

		file.InitForRead( path );

		if( !file.IsValid() )
		{
			log::error << "can not open resource binary: " << path << std::endl;
			return 0;
		}

        bool*       fp_hasPosition = (bool*) file.Data();
        bool*       fp_hasNormals = &fp_hasPosition[1];
        uint32_t*   fp_arraySize = (uint32_t*) &fp_hasPosition[2];
        uint32_t*   fp_numUvChannels = &fp_arraySize[1];
        uint32_t*   fp_indiciesCount = &fp_numUvChannels[1];
        float*      fp_vertexData = (float*) &fp_indiciesCount[1];
        int*        fp_indiciesData = (int*) &fp_vertexData[*fp_arraySize];
        
        const uint32_t vertexfields = (*fp_hasPosition ? 3 : 0)
                                + (*fp_hasNormals ? 3 : 0)
                                + *fp_numUvChannels * 2;

        const uint32_t atributesInUse = (*fp_hasPosition ? 1 : 0)
                                + (*fp_hasNormals ? 1 : 0)
                                + *fp_numUvChannels;



       	uint32_t* config = new uint32_t[atributesInUse*5+4];
       	uint32_t index = 3;

        config[2] = atributesInUse; // present attributes count

        glGenBuffers(1, &config[0]);
        glBindBuffer(GL_ARRAY_BUFFER, config[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * *fp_arraySize * vertexfields, 0, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * *fp_arraySize * vertexfields, fp_vertexData);


        glGenBuffers(1, &config[1]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, config[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* *fp_indiciesCount, NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)* *fp_indiciesCount, fp_indiciesData);


       int attrPtrOffset = 0;

        if(fp_hasPosition)
        {
            config[index] = ATTR_LOC_position; 
            ++index;
            config[index] = 0; 
            ++index;
            config[index] = 3; 
            ++index;
            config[index] = vertexfields; 
            ++index;
            config[index] = attrPtrOffset;
            attrPtrOffset += 3;
            ++index;
            //glEnableVertexAttribArray(0);
            //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*9, nullptr);
        }

        if(*fp_hasNormals)
        {
            config[index] = ATTR_LOC_normal;
            ++index;
            config[index] = 0;
            ++index;
            config[index] = 3;
            ++index;
            config[index] = vertexfields;
            ++index;
            config[index] = attrPtrOffset;
            attrPtrOffset += 3;
            ++index;
        }

        for(uint32_t UVchannel = 0; UVchannel<*fp_numUvChannels; ++UVchannel)
        {
            config[index] = ATTR_LOC_uv0 + UVchannel;
            ++index;
            config[index] = 2;
            ++index;
            config[index] = 2;
            ++index;
            config[index] = vertexfields;
            ++index;
            config[index] = attrPtrOffset;
            attrPtrOffset += 2;
            ++index;
            // glEnableVertexAttribArray(2);
            // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*9, (void*) (sizeof(GL_FLOAT)*3) );
        }

        config[index] = *fp_indiciesCount;

        return (void*)config;
	}

	void ResourceFBXProcessor::UnloadResource(void* handle)
	{
		uint32_t* config = (uint32_t*)handle;
		if( config != nullptr)
		{
			glDeleteBuffers(1, &config[0]);
			glDeleteBuffers(1, &config[1]);
			delete config;
		}
	}


	bool ResourceFBXProcessor::ProcessResource2Binary(const asapi::ResourceTracker& in_currentResource
											, asapi::FILE::MMAP* in_resourceFile
											, const char* in_projectPath
											, std::vector<asapi::SubResourceData>* out_resourceBinaries)
	{
		out_resourceBinaries->clear();
		in_resourceFile->Close();


		loadModel( in_currentResource.GetFullPath()
					, in_currentResource
					, in_resourceFile
					, in_projectPath
					, out_resourceBinaries );

		return true;
	}


	const char* ResourceFBXProcessor::GetSuportedResourceFileExtension()
	{
		return ".fbx";
	}

	const char* ResourceFBXProcessor::GetBinaryOutputFileExtension()
	{
		return ".mesh";
	}


















    void ProcessMesh(aiMesh *mesh
					, const asapi::ResourceTracker& in_currentResource
					, asapi::FILE::MMAP* in_resourceFile
					, const char* in_projectPath
					, std::vector<asapi::SubResourceData>* out_resourceBinaries)
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




		asapi::UniqueID subresourceID;
		std::string subresourcePath;

		bool subresourcePreviouslyExisted = in_currentResource.FindSubResourceByInternalID( std::string( mesh->mName.C_Str() ), subresourceID );




		//////////////////////////////////////////////////////////////////////////////////////////////////
		//																								//
		//																								//
		//																								//
		//																								//
		//				This if section needs to be reimplemented in custom implementations.			//
		//				Otherwise binary resource name will not be tranfered between updates			//
		//																								//
		//																								//
		//																								//
		//																								//
		//////////////////////////////////////////////////////////////////////////////////////////////////

		
		std::string binaryResourceDir = in_projectPath;
		binaryResourceDir += RESOURCE_BINARIES_DIR;
		binaryResourceDir += "/";

		subresourcePath = std::to_string( (uint64_t)subresourceID ) 
						+ std::string( ResourceFBXProcessor::GetBinaryOutputFileExtension() ) 
						+ std::string( ".bin" );


		subresourcePath = binaryResourceDir + subresourcePath;


		asapi::SubResourceData subresource(
								subresourceID
								, std::string( subresourcePath.c_str() ));
		out_resourceBinaries->push_back( subresource );

		//Star of actual subresource copmpilation section:






        //allocte memory maped file to convert the data
        SYSTEMS::IO::MMAP mmap;
        mmap.InitForWrite(subresourcePath.c_str(),
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

    void processNode(aiNode *node, const aiScene *scene
					, const asapi::ResourceTracker& in_currentResource
					, asapi::FILE::MMAP* in_resourceFile
					, const char* in_projectPath
					, std::vector<asapi::SubResourceData>* out_resourceBinaries)
    {
        // process each mesh located at the current node
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            ProcessMesh(mesh
					, in_currentResource
					, in_resourceFile
					, in_projectPath
					, out_resourceBinaries);
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene
					, in_currentResource
					, in_resourceFile
					, in_projectPath
					, out_resourceBinaries);
        }

    }

	void loadModel(std::string path
					, const asapi::ResourceTracker& in_currentResource
					, asapi::FILE::MMAP* in_resourceFile
					, const char* in_projectPath
					, std::vector<asapi::SubResourceData>* out_resourceBinaries)
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

		processNode(scene->mRootNode, scene
					, in_currentResource
					, in_resourceFile
					, in_projectPath
					, out_resourceBinaries);
	}
}