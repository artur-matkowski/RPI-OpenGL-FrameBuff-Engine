#include "TestResource.hxx"

//bool (*callback)(ResourceTracker*, void*
bool ProcessInputResourceToBinary(asapi::ResourceTracker* restrack, void* data)
{
	asapi::FILE::MMAP _in, _out;
	const std::string binaryResourceDir = std::string( (const char*)data ) + std::string("Resource_Binaries/");
	const std::string binaryResource = binaryResourceDir + std::to_string( restrack->GetResourceID() ) + std::string(".txt.bin");


	_in.InitForRead( restrack->GetPath().c_str() );

	_out.InitForWrite( binaryResource.c_str(), 64);


	strncpy( (char*)_out.Data(), (const char*)_in.Data(), _in.Size() );

	return true;
}