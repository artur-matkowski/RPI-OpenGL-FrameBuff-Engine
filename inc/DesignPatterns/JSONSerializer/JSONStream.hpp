#ifndef _H_JSONStream
#define _H_JSONStream
#include "PrecompiledHeaders.hpp"
#include "SerializableBase.hpp"
#include <stdio.h>
#include <cstring>

template<class T>
class SerializableVarVector;

class JSONStream{
  int buffsize = 1024*4;
  char* first = 0;
  char* last = 0;
  char* current = 0;

  inline bool isOneOf(char* c, const char* str)
  {
    int size = strlen(str);
    
    for(int i=0; i<size; ++i)
    {
      if(*c==str[i])
      {
        return true;
      }
    }
    return false;
  }

public:
  inline void skipToOneOf(const char* str)
  {
    while( current!=last && !isOneOf(current, str) ) {
      ++current;
    }
  }

  inline void skipTo(char c)
  {
    while( current!=last && *current!=c ) {
      ++current;
    }
  }
  inline void skip(int c)
  {
    if( (current + c) <= last )
    {
      current += c;
    }
    else
    {
      current = last;
    }
  }

  JSONStream()
    :first(new char[buffsize])
    ,last(first+buffsize-1)
    ,current(first)
    {
      memset(first, 0, buffsize);
    }

  JSONStream(const char* input)
    {
      int insize = strlen (input);
      buffsize = std::max(buffsize, insize);

      first = new char[buffsize];
      last = first+buffsize-1;
      current = first;

      std::memset(first, 0, buffsize);
      std::memcpy(first, input, insize);
    }

  ~JSONStream()
  {
    delete[] first;
  }

  JSONStream& Deserialize(float& val)
  {
    skipToOneOf("-.0123456789");

    current += sscanf(current, "%f", &val);
    skipToOneOf(",]");

    return *this;
  }

  JSONStream& Deserialize(int& val)
  {
    skipToOneOf("-0123456789");

    current += sscanf(current, "%d", &val);
    skipToOneOf(",]");

    return *this;
  }

  JSONStream& Deserialize(bool& val)
  {
    skipToOneOf("tf");

    if( 0==strncmp(current, "true", 4) )
    {
      val = true;
    }
    else if( 0==strncmp(current, "false", 5) )
    {
      val = false;
    }
    
    skipToOneOf(",]");

    return *this;
  }

  JSONStream& Deserialize(std::string& val)
  {
    skipTo('\"');

    char* skipper = current+1;

    while( current!=last && *skipper != '\"' )
    {
      if(*skipper=='\\' )
      {
        ++skipper;
        if( current==last )
          break;
      }
      ++skipper;
    }
    
    val.assign(current+1, skipper);

    current = skipper;
    skipToOneOf(",]");

    return *this;
  }

  JSONStream& operator>> ( SerializableBase& val)
  {
    val.Deserialize(*this);

    return *this;
  }




  JSONStream& Serialize(const float& val)
  {
    current += snprintf(current, last-current, "%f", val);
    return *this;
  }

  JSONStream& Serialize(const int& val)
  {
    current += snprintf(current, last-current, "%d", val);
    return *this;
  }

  JSONStream& Serialize(const bool& val)
  {
    current += snprintf(current, last-current, "%s", (val ? "true" : "false") );
    return *this;
  }

  JSONStream& Serialize(const std::string& val)
  {
    current += snprintf(current, last-current, "\"%s\"", val.c_str() );
    return *this;
  }

  JSONStream& operator<< ( SerializableBase& val)
  {
    val.Serialize(*this);

    return *this;
  }



  std::string str()
  {
    return std::string(first,last);
  }

  void SetCursonPos(int pos)
  {
    current = first + pos;
  }

  void sprintf(const char* str, ...)
  {
    va_list args;
    va_start(args, str);

    current += vsnprintf(current, last-current, str, args);
    va_end(args);
  }

  inline char peak() const
  {
    return *current;
  }
};

#endif