#pragma once

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <map>
#include <cstring>
#include <sstream>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#define SPDLOG_COMPILED_LIB 1
#include <spdlog/spdlog.h>

//for dll export
#if defined(_ROOMENG_BUILD_DLL)
	#define ROOMENGAPI __declspec(dllexport)
#else
	#define ROOMENGAPI __declspec(dllimport)
#endif

namespace RoomEng{
	namespace Utils{
		template <typename T>
		void addBytesToString(std::string& theString, const T* object){
			char arr[sizeof(T)];
			std::memcpy(arr, object, sizeof(T));
			theString += std::string(arr);
		}

		glm::mat4 aiMat2glmMat(aiMatrix4x4 mat);
	}
}