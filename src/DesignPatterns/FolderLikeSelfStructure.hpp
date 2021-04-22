#ifndef H_FolderLikeSelfStructure
#define H_FolderLikeSelfStructure
#include <vector>
#include <cstring>
#include "Systems.hpp"



template<class InfoBufferType>
class Node
{
public:
	InfoBufferType m_infoBuffer;
protected:
	char m_name[MAX_PATH_SIZE] = {'\0'};

	std::vector<Node, bfu::custom_allocator<Node>> v_children;

	bool HasChildren()
	{
		return v_children.size() != 0;
	}

	inline bool IsDelimeter(const char c, const char* delimeters_v, const int delimeters_c)
	{
		for(int i=0; i<delimeters_c; ++i)
		{
			if( c==delimeters_v[i] )
			{
				return true;
			}
		}
		return false;
	}

	inline const char* TruncLeadingDelimeters(const char* path, const char* delimeters_v, int & path_c, const int delimeters_c)
	{
		for(int i=0; i<path_c; ++i)
		{
			if( !IsDelimeter(path[i], delimeters_v, delimeters_c) )
			{
				path_c -= i; 
				return path+i;
			}
		}
		return nullptr;
	}

public:
	Node(const char* in_name, const InfoBufferType& in_info)
		:m_infoBuffer(in_info)
		,v_children(asapi::SYSTEMS::SYSTEMS_ALLOCATOR)
	{
		const int in_name_c = strlen(in_name);
		strncpy(m_name, in_name, in_name_c < MAX_PATH_SIZE ? in_name_c : MAX_PATH_SIZE);
	}

	Node(const char* in_name, const int in_name_c, const InfoBufferType& in_info)
		:m_infoBuffer(in_info)
		,v_children(asapi::SYSTEMS::SYSTEMS_ALLOCATOR)
	{
		strncpy(m_name, in_name, in_name_c < MAX_PATH_SIZE ? in_name_c : MAX_PATH_SIZE);
	}

	Node(const Node& cp)
		:m_infoBuffer(cp.m_infoBuffer)
		,v_children(asapi::SYSTEMS::SYSTEMS_ALLOCATOR)
	{
		strncpy(m_name, cp.m_name, MAX_PATH_SIZE );
		v_children = cp.v_children;
	}

	Node()
		:v_children(asapi::SYSTEMS::SYSTEMS_ALLOCATOR)
	{};

	inline void FeedEntry(const char* path, const char* delimeters_v, const InfoBufferType& in_info)
	{
		FeedEntry(path, delimeters_v, strlen(path), strlen(delimeters_v), in_info);
	}

	inline void FeedEntry(const char* path, const char* delimeters_v, int path_c, const InfoBufferType& in_info)
	{
		FeedEntry(path, delimeters_v, path_c, strlen(delimeters_v), in_info);
	}

	inline void FeedEntry(const char* path, const char* delimeters_v, int path_c, const int delimeters_c, const InfoBufferType& in_info)
	{
		m_infoBuffer = in_info;

		path = TruncLeadingDelimeters(path, delimeters_v, path_c, delimeters_c);

		if(path_c<=0)
			return;

		for(int i=0; ; ++i)
		{
			if( IsDelimeter( path[i], delimeters_v, delimeters_c ) || path[i]=='\0' )
			{
				path += i;
				path_c -= i;
				m_name[i] = '\0';
				break;
			}
			m_name[i] = path[i];
		}

		if(path_c<=0)
			return;


		this->operator[](m_name)->FeedEntry(path, delimeters_v, path_c, delimeters_c, in_info);
	}




	Node* operator[](const char* token)
	{
		for(auto it = v_children.begin(); it!=v_children.end(); ++it)
		{
			if( strcmp(token, it->m_name) == 0 )
			{
				return &*it;
			}
		}

		//if not found, add a new entry
		v_children.emplace_back( token, m_infoBuffer );
		return &v_children.back();
	}

	Node* operator[](const int i)
	{
		return &v_children[i];
	}

	const int size() const
	{
		return v_children.size();
	}

	const char* name() const
	{
		return m_name;
	}
};




#endif