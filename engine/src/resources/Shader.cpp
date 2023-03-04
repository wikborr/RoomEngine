#include "resources/Shader.h"

namespace RoomEng{
	Shader::~Shader(){
		if(this->programID != 0){
			glDeleteProgram(this->programID);
		}
	}

	void Shader::compileShaders(std::string vertexString, std::string fragmentString, std::string geometryString){
		const char* vString = vertexString.c_str();
		const char* fString = fragmentString.c_str();
		const char* gString = geometryString.c_str();

		unsigned vShader = 0, fShader = 0, gShader = 0;
		int success;
		char infoLog[1024];

		vShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vShader, 1, &vString, nullptr);
		glCompileShader(vShader);
		glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
		if(!success){
			glGetShaderInfoLog(vShader, 1024, NULL, infoLog);
			spdlog::error("ERROR: vertex shader: compilation error:");
			spdlog::error(infoLog);
		}

		fShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fShader, 1, &fString, nullptr);
		glCompileShader(fShader);
		glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
		if(!success){
			glGetShaderInfoLog(fShader, 1024, NULL, infoLog);
			spdlog::error("ERROR: fragment shader: compilation error:");
			spdlog::error(infoLog);
		}

		if(geometryString != ""){
			gShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(gShader, 1, &gString, nullptr);
			glCompileShader(gShader);
			glGetShaderiv(gShader, GL_COMPILE_STATUS, &success);
			if(!success){
				glGetShaderInfoLog(gShader, 1024, NULL, infoLog);
				spdlog::error("ERROR: geometry shader: compilation error:");
				spdlog::error(infoLog);
			}
		}

		if(this->programID != 0){
			glDeleteProgram(this->programID);
		}
		this->programID = glCreateProgram();
		glAttachShader(this->programID, vShader);
		glAttachShader(this->programID, fShader);
		if(geometryString != ""){
			glAttachShader(this->programID, gShader);
		}
		glLinkProgram(this->programID);
		glGetProgramiv(this->programID, GL_LINK_STATUS, &success);
		if(!success){
			glGetProgramInfoLog(this->programID, 1024, NULL, infoLog);
			spdlog::error("ERROR: shader program: linking error:");
			spdlog::error(infoLog);
		}
		glDeleteShader(vShader);
		glDeleteShader(fShader);
		if(geometryString != ""){
			glDeleteShader(gShader);
		}
	}
}