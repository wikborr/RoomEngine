#include "Window.h"
#include <stb_image.h>
#include "Settings.h"
#include "Renderer.h"
#include "Input.h"

namespace RoomEng{
	Window::Window(std::string projectName, int width, int height, bool windowed){
		this->defaultScreenWidth = width;
		this->screenWidth = width;
		this->defaultScreenHeight = height;
		this->screenHeight = height;

		//GLFW setup
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		this->window = glfwCreateWindow(this->screenWidth, this->screenHeight, projectName.c_str(), nullptr, nullptr);
		if(this->window == nullptr){
			spdlog::error("GLFW: window creation failed");
			exit(0);
		}
		glfwMakeContextCurrent(this->window);
		if(FileReader::fileExists("assets/icon.png")){
			this->icon[0].pixels = stbi_load("assets/icon.png", &this->icon[0].width, &this->icon[0].height, 0, 4);
			glfwSetWindowIcon(this->window, 1, this->icon); 
			stbi_image_free(this->icon[0].pixels);
		}

		//OpenGL setup
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
			spdlog::error("OpenGL: initialization failed");
			exit(0);
		}
		glViewport(0, 0, this->screenWidth, this->screenHeight);
		glfwSetFramebufferSizeCallback(this->window, [](GLFWwindow* window, int width, int height)->void{
			Settings::ins().window.framebufferSizeCallback(window, width, height);
		});
		glfwSetErrorCallback([](int error, const char* description)->void{
			Settings::ins().window.errorCallback(error, description);
		});
		
		if(!windowed){
			this->goFullscreen();
		}
	}

	void Window::goFullscreen(){
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		if(this->fullscreen){
			glfwSetWindowMonitor(this->window, nullptr, this->screenPosX, this->screenPosY, this->screenWidth, this->screenHeight, GLFW_DONT_CARE);
			this->fullscreen = false;
		}
		else{
			this->fullscreen = true;
			glfwGetWindowPos(this->window, &this->screenPosX, &this->screenPosY);
			glfwSetWindowMonitor(this->window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
		}
	}

	void Window::destroy(){
		Input::ins().changeMouseMode(MouseMode::Visible);
		glfwDestroyWindow(this->window);

		glfwSetKeyCallback(Settings::ins().window.window, [](GLFWwindow* window, int key, int scancode, int action, int mods)->void{});
		glfwSetCursorPosCallback(Settings::ins().window.window,  [](GLFWwindow* window, double xpos, double ypos)->void{});
		glfwSetCursorEnterCallback(Settings::ins().window.window,  [](GLFWwindow* window, int entered)->void{});
		glfwSetMouseButtonCallback(Settings::ins().window.window,  [](GLFWwindow* window, int button, int action, int mods)->void{});
		glfwSetScrollCallback(Settings::ins().window.window,  [](GLFWwindow* window, double xoffset, double yoffset)->void{});
		glfwSetWindowFocusCallback(Settings::ins().window.window, [](GLFWwindow* window, int focused)->void{});
		glfwSetFramebufferSizeCallback(this->window, [](GLFWwindow* window, int width, int height)->void{});

		glfwTerminate();
	}

	void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height){
		glViewport(0, 0, width, height);
		if(!fullscreen){
			this->screenWidth = width;
			this->screenHeight = height;
		}
		if(this->engineInitialized){
			Renderer::ins().render();
		}
	}

	void Window::errorCallback(int error, const char* description){
		spdlog::error("GLFW error:\n"+std::string(description));
	}
}