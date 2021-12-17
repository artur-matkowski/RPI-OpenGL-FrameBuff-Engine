#include "TestResource.hxx"


bool ProcessResourceTracker(asapi::ResourceTracker* in_currentResource, const char* in_projectPath, std::vector<asapi::SubResourceData>& out_resourceBinaries)
{
	asapi::FILE::MMAP _in, _out;

	std::string binaryResourcePath = std::string( in_projectPath );
		binaryResourcePath += std::string( RESOURCE_BINARIES_DIR );
		binaryResourcePath += std::to_string( restrack->GetResourceID() );
		binaryResourcePath +=  std::string(".txt.bin");


	std::string resourcePath = in_currentResource->GetFullPath();


	_in.InitForRead( resourcePath.c_str() );

	_out.InitForWrite( binaryResourcePath.c_str(), 64);


	strncpy( (char*)_out.Data(), (const char*)_in.Data(), _in.Size() );

	out_resourceBinaries.emplace_back( std::to_string( restrack->GetResourceID() ), "--no id needed--")

	return true;
}