#include "AssetMetaDataSocket.hpp"
#include "Systems.hpp"
#include "sha256.h"


namespace asapi
{
	AssetMetaDataSocket::AssetMetaDataSocket(bfu::MemBlockBase* mBlock)
		:m_assetFileName(m_fileNameBuff, MAX_FILENAME_SIZE, mBlock)
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


	bool AssetMetaDataSocket::IsDirty(const char* path)
	{
		struct stat attribInt;
		struct stat attribExt;
		char metadataPath[MAX_PATH_SIZE];


		strncpy(metadataPath, path, MAX_PATH_SIZE-1);
		metadataPath[MAX_PATH_SIZE-1] = '\0';

		RemoveExtensions(metadataPath);



		strcpy(metadataPath+strlen(metadataPath), ".asset.json");


		bool ret = false;
    	if( stat(path, &attribExt)!=0 )
    	{
    		log::error << "Could not find file " << path << std::endl;
    	}
    	else if( stat(metadataPath, &attribInt)!=0 )
    	{
    		log::info << "Could not find asset metadata file " << metadataPath << " asset metadata will be generated" << std::endl;
    	}
    	else
    	{
    		ret = attribExt.st_mtime < attribInt.st_mtime;
    	}
    	return ret;
	}
	std::string AssetMetaDataSocket::GetHash(const char* path)
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


		log::debug << "has of file: " << path << " : " << sha256.getHash() << std::endl; 

		return sha256.getHash();
	}
}