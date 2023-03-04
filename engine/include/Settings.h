#pragma once
#include "Utils.h"
#include "FileReader.h"
#include "Window.h"

namespace RoomEng{
	//class that contains global project settings
	class ROOMENGAPI Settings{
	public:
		//singleton method
		Settings(){}
		static Settings& ins();

		Window window;
		int fpsLimit = 0;
		bool debugCollision = false;
		double time = 0.0;

		void init(std::string projectName, int width, int height, bool windowed);
		float getTime();
		void close();
	
	};
}