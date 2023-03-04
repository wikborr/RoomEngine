#pragma once
#include "Utils.h"
#include "FileReader.h"

namespace RoomEng{
	//class managing the window
	class ROOMENGAPI Window{
	public:
		Window(){}
		Window(std::string projectName, int width, int height, bool windowed);
		
		GLFWwindow* window = nullptr;
		bool fullscreen= false;
		int screenPosX = 100;
		int screenPosY = 100;
		int defaultScreenWidth;
		int defaultScreenHeight;
		int screenWidth;
		int screenHeight;
		std::string projectName;
		glm::vec3 bgColor = glm::vec3(0.0f, 0.33f, 0.33f);
		GLFWimage icon[1];
		bool engineInitialized = false;

		//goes into or exits the fullscreen
		void goFullscreen(); 
		void destroy();

		//callback functions
		void framebufferSizeCallback(GLFWwindow* window, int width, int height);
		void errorCallback(int error, const char* description);
	};
}