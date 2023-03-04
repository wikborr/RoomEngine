#include "Settings.h"

namespace RoomEng{
	Settings& Settings::ins(){
		static Settings instance;
		return instance;
	}

	void Settings::close(){
		if(this->window.window != nullptr){
			glfwSetWindowShouldClose(this->window.window, true);
		}
		else{
			spdlog::critical("No window detected, exit is impossible.");
		}
	}

	void Settings::init(std::string projectName, int width, int height, bool windowed){
		this->window = Window(projectName, width, height, windowed);
	}

	float Settings::getTime(){
		return static_cast<float>(this->time);
	}
}