#include "Core.h"
#include "Settings.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Input.h"

namespace RoomEng{
	void Core::init(Scene* scene, std::string projectName, int width, int height, bool windowed){
		if(scene == nullptr){
			spdlog::error("Scene not set");
			return;
		}
		this->scene = scene;

		//pseudo random numbers seed
		srand(time(0));
		
		//Settings and Window init
		Settings::ins().init(projectName, width, height, windowed);

		//Resource initialization
		ResourceManager::ins().init();

		//Renderer initialization
		Renderer::ins().init(scene);

		//Physics initialization

		//Input initialization
		Input::ins().init();

		//Scene initialization
		this->scene->doReadies();
		this->scene->doObjectOperations2();
		
		//starting the main loop
		Settings::ins().window.engineInitialized = true;
		spdlog::info("Starting the main loop...");
		spdlog::info("--------------------------");
		double lastTime = 0.0;
		while(!glfwWindowShouldClose(Settings::ins().window.window)){
			//delta
			double time = glfwGetTime();
			double deltaTime = time - lastTime;
			lastTime = time;
			Settings::ins().time = time;

			//logic updates
			this->scene->doObjectOperations1(deltaTime);
			this->scene->doUpdates(deltaTime);
			this->scene->doObjectOperations2(deltaTime);

			//rendering
			Renderer::ins().render();

			//input updates
			Input::ins().removeJustKeys();

			//check for GLFW events
			glfwPollEvents();

			//fps limit
			if(Settings::ins().fpsLimit > 0){
				//int timeLeft = static_cast<int>((lastTime + (1.0/(Settings::ins().fpsLimit+1)) - glfwGetTime())*1000000);
				while (glfwGetTime() < lastTime + 1.0/(Settings::ins().fpsLimit+1)) {}
			}
		}
		spdlog::info("--------------------------");
		//deallocation
		Settings::ins().window.destroy();
		spdlog::info("GLFW deallocation complete");
		delete this->scene;
		spdlog::info("RoomObjects deallocation complete");
		ResourceManager::ins().destroy();
		spdlog::info("Resource deallocation complete");
		Renderer::ins().destroy();
		spdlog::info("OpenGL context deallocation complete");
	}
}