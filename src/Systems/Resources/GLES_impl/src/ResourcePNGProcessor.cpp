#include "ResourcePNGProcessor.hpp"
#include "ResourceSystem.hpp"
#include "bfu.hpp"

namespace asapi
{
	std::vector<std::string*> ResourcePNGProcessor::data;
	
	void* ResourcePNGProcessor::LoadResource(const char* path)
	{
		asapi::FILE::MMAP file;

		file.InitForRead( path );

		if( !file.IsValid() )
		{
			log::error << "can not open resource binary: " << path << std::endl;
			return 0;
		}

		data.emplace_back( new std::string( (const char*)file.Data()) );

		return (void*)data.back()->c_str();
	}

	void ResourcePNGProcessor::UnloadResource(void* handle)
	{
		for(auto it = data.begin(); it!=data.end(); it++)
		{
			if( (*it)->c_str() == handle )
			{
				delete *it;
				data.erase( it );
				break;
			}
		}
	}


	bool ResourcePNGProcessor::ProcessResource2Binary(const asapi::ResourceTracker& in_currentResource
											, asapi::FILE::MMAP* in_resourceFile
											, const char* in_projectPath
											, std::vector<asapi::SubResourceData>* out_resourceBinaries)
	{
		std::string binaryResourceDir = in_projectPath;
		binaryResourceDir += RESOURCE_BINARIES_DIR;
		binaryResourceDir += "/";

		out_resourceBinaries->clear();

		//const std::string binaryResource = binaryResourceDir + std::to_string( in_currentResource->GetResourceID() ) + std::string(".txt.bin");


		std::string databuff( (char*)in_resourceFile->Data(), (char*)in_resourceFile->Data()+in_resourceFile->Size() );
		std::istringstream iss(databuff);

		std::string line;
		int i = 0;
		while (std::getline(iss, line))
		{
			asapi::FILE::MMAP _out;
			asapi::UniqueID subresourceID;
			std::string binaryResource;

			bool subresourcePreviouslyExisted = in_currentResource.FindSubResourceByInternalID( std::to_string(i), subresourceID );




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

			binaryResource = std::to_string( (uint64_t)subresourceID ) + ".txt.bin";

			asapi::SubResourceData subresource(
									subresourceID
									, std::to_string(i));

			binaryResource = binaryResourceDir + binaryResource;

			_out.InitForWrite( binaryResource.c_str(), in_resourceFile->Size());
			memcpy( _out.Data(), (void*)line.c_str(), line.size() );

			i++;
			out_resourceBinaries->push_back( subresource );
		}

		return true;
	}


	const char* ResourcePNGProcessor::GetSuportedResourceFileExtension()
	{
		return ".txt";
	}

	const char* ResourcePNGProcessor::GetBinaryOutputFileExtension()
	{
		return ".txt";
	}


	bfu::stream& operator<<(bfu::stream& st, const ResourcePNGProcessor& out )
	{
		st << "\n\tResourcePNGProcessor::data:";

		for(int i=0; i<ResourcePNGProcessor::data.size(); i++)
		{
			st << "\n\t\t" << ResourcePNGProcessor::data[i]->c_str();
		}

		return st;
	}
}