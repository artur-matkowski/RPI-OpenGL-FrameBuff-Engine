#ifndef _H_SerializableVarVector
#define _H_SerializableVarVector
#include "JSONSerializerBase.hpp"
#include "SerializableVar.hpp"


namespace asapgl
{
	template<class T>
	class SerializableVarVector: public std::vector<SerializableVar<T>>, public SerializableBase
	{
		SerializableVarVector()
			:std::vector<SerializableVar<T>>()
		{}
	public:

		SerializableVarVector(const char* Name, SerializableClassBase* parent)
			:std::vector<SerializableVar<T>>()
		{
			if(parent!=0)
				parent->PushReferenceToMap(Name, this);
		}


		//TODO optimize me:
		SerializableVarVector<T>& operator=(const std::vector<T>& in)
		{
			this->clean();

			for(auto it = in.begin(); it != in.end(); ++it)
			{
				this->push_back(*it);
			}

			return *this;
		}

		virtual std::string Serialize()
		{
			std::stringstream ret;
			ret << "[";

			int size = this->size();

			for(int i=0; i<size; ++i)
			{
				ret << this->operator[](i).Serialize();

				if(i < size-1)
				{
					ret << ", ";
				}
			}

			ret << "]";

			return ret.str();
		}

		virtual void Deserialize(const char* str, const int size)
		{
			this->clear();

			T val;
			std::string tmp; 
			std::istringstream ss(str+1);
			std::vector<std::string> words;

			while(getline(ss, tmp, ',')){
				std::istringstream iss(tmp);

				iss >> val;

				this->push_back(val);
			}
		}
	};

}

#endif