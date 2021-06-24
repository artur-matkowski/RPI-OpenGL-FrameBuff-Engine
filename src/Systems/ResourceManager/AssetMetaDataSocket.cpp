#include "AssetMetaDataSocket.hpp"
#include "Systems.hpp"
#include "sha256.h"


namespace asapi
{
	AssetMetaDataSocket::AssetMetaDataSocket()
	{

	}


	void RemoveExtensions(char* in)
	{
		const int ln = strlen(in);

		const int size = MAX_PATH_SIZE < ln ? MAX_PATH_SIZE : ln;

		for(int j=size-1; j>0; --j)
		{
			if(in[j]=='.')
				in[j] = '\0';
			else if(in[j]=='/')
				break;
		}
	}


	eAssetImportType AssetMetaDataSocket::AssetImportState(const char* path)
	{
		struct stat attribInt;
		struct stat attribExt;
		char metadataPath[MAX_PATH_SIZE];


		strncpy(metadataPath, path, MAX_PATH_SIZE-1);
		metadataPath[MAX_PATH_SIZE-1] = '\0';

		RemoveExtensions(metadataPath);



		strcpy(metadataPath+strlen(metadataPath), ".asset.json");


		eAssetImportType ret;
    	if( stat(path, &attribExt)!=0 )
    	{
    		log::error << "Could not find file " << path << std::endl;
    		ret = Invalid;
    	}
    	else if( stat(metadataPath, &attribInt)!=0 )
    	{
    		log::info << "Could not find asset metadata file " << metadataPath << " asset metadata will be generated" << std::endl;
    		ret = BrandNew;

    		AssetMetaDataSocket* asset = SYSTEMS::GetObject().RESOURCES.GetAssetMetaDataSocketByHash( AssetMetaDataSocket::GetHash(path) );
    		if( asset!=nullptr )
    		{
    			ret = Moved;
    		}
    	}
    	else
    	{
    		ret = attribExt.st_mtime < attribInt.st_mtime ? Updated : Untouched;
    	}
    	return ret;
	}

	string AssetMetaDataSocket::GetHash(const char* path)
	{
		struct stat attrib;

		if( stat(path, &attrib)!=0 )
    	{
    		log::error << "Could not find file " << path << std::endl;
    	}

    	SYSTEMS::IO::MMAP mmap;
        mmap.InitForRead(path);

		SHA256 sha256; 
		sha256(mmap.Data(), mmap.Size());


		log::debug << "hash of file: " << path << " : " << sha256.getHash() << std::endl; 

		return sha256.getHash();
	}


	void AssetMetaDataSocket::OnAssetMoved(const char* newPath, AssetMetaDataSocket * asset)
	{
		log::debug << "OnAssetMoved: " << newPath << std::endl; 
	}
	AssetMetaDataSocket AssetMetaDataSocket::OnAssetAdded(const char* path, const string & hash)
	{
		log::debug << "OnAssetAdded: " << path << std::endl;

		char buff[MAX_PATH_SIZE];
		strncpy(buff, path, MAX_PATH_SIZE-1);
		RemoveExtensions(buff);
		strncat(buff, ".asset.json", MAX_PATH_SIZE-1);

		AssetMetaDataSocket asset;
		asset.m_hash = hash;
		const char* relPath = strstr(path, "/assets/") + strlen("/assets/");
		asset.m_assetPath = string(relPath);

		SYSTEMS::IO::STREAM file;
		file.InitForWrite(path);

		bfu::JSONSerializer &jsonSerializer = SYSTEMS::GetObject().SCENE.GetJSONSerializer();
		jsonSerializer.clear();

		jsonSerializer.Serialize( &asset );

		SceneSystem::JSON2File( jsonSerializer, buff ) ;


		return asset;
	}
}