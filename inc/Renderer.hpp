#ifndef _H_RENDERER
#define _H_RENDERER
#include "object.hpp"

namespace asapgl{

	class Renderer: public object
	{
	public:
		Renderer();
		~Renderer();

		void Init();


		void Update(float deltaTime);
	};

}


#endif