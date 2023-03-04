#pragma once
#include "resources/Resource.h"

namespace RoomEng{
	//resource class for shaders
	class ROOMENGAPI Shader: public Resource{
	public:
		~Shader();
		unsigned programID = 0;

		void compileShaders(std::string vertexString, std::string fragmentString, std::string geometryString = "");
	};
}