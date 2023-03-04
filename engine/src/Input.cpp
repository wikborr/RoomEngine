#include "Input.h"

namespace RoomEng{
	Input& Input::ins(){
		static Input instance;
		return instance;
	}
	
	void Input::init(){
		this->keys.fill(std::pair<bool, bool>(false,false));
		this->mouseKeys.fill(std::pair<bool, bool>(false,false));
		this->window_focus_callback(Settings::ins().window.window, glfwGetWindowAttrib(Settings::ins().window.window, GLFW_FOCUSED));
		this->cursor_enter_callback(Settings::ins().window.window, glfwGetWindowAttrib(Settings::ins().window.window, GLFW_HOVERED));
		
		glfwSetKeyCallback(Settings::ins().window.window, [](GLFWwindow* window, int key, int scancode, int action, int mods)->void{
			Input::ins().key_callback(window, key, scancode, action, mods);
		});
		glfwSetCursorPosCallback(Settings::ins().window.window,  [](GLFWwindow* window, double xpos, double ypos)->void{
			Input::ins().cursor_position_callback(window, xpos, ypos);
		});
		glfwSetCursorEnterCallback(Settings::ins().window.window,  [](GLFWwindow* window, int entered)->void{
			Input::ins().cursor_enter_callback(window, entered);
		});
		glfwSetMouseButtonCallback(Settings::ins().window.window,  [](GLFWwindow* window, int button, int action, int mods)->void{
			Input::ins().mouse_button_callback(window, button, action, mods);
		});
		glfwSetScrollCallback(Settings::ins().window.window,  [](GLFWwindow* window, double xoffset, double yoffset)->void{
			Input::ins().scroll_callback(window, xoffset, yoffset);
		});
		glfwSetWindowFocusCallback(Settings::ins().window.window, [](GLFWwindow* window, int focused)->void{
			Input::ins().window_focus_callback(window, focused);
		});

		//changeMouseMode(this->mouseMode);
	}

	void Input::removeJustKeys(){
		for(auto& i: this->keys){
			if(i.first && !i.second){
				i.second = true;
			}
			else if(!i.first && i.second){
				i.second = false;
			}
		}
		for(auto& i: this->mouseKeys){
			if(i.first && !i.second){
				i.second = true;
			}
			else if(!i.first && i.second){
				i.second = false;
			}
		}
		this->mouseScrollOffset = glm::vec2(0.0f, 0.0f);
		this->prevMousePos = this->mousePos;
	}

	void Input::changeMouseMode(MouseMode newMouseMode){
		if(newMouseMode != this->mouseMode) {
			this->mouseMode = newMouseMode;
			if (newMouseMode == MouseMode::Visible) {
				glfwSetInputMode(Settings::ins().window.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				if (glfwRawMouseMotionSupported()) {
					glfwSetInputMode(Settings::ins().window.window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
				}
			}
			else if (newMouseMode == MouseMode::Hidden) {
				glfwSetInputMode(Settings::ins().window.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				if (glfwRawMouseMotionSupported()) {
					glfwSetInputMode(Settings::ins().window.window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
				}
			}
			else if (newMouseMode == MouseMode::Captured) {
				glfwSetInputMode(Settings::ins().window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				if (glfwRawMouseMotionSupported()) {
					glfwSetInputMode(Settings::ins().window.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
				}
			}
		}
	}

	bool Input::isKeyJustPressed(int key){
		if(key<0) return false;
		if(this->keys.at(key).first && !this->keys.at(key).second) return true;
		return false;
	}
	bool Input::isKeyPressed(int key){
		if(key<0) return false;
		if(this->keys.at(key).first) return true;
		return false;
	}
	bool Input::isKeyJustReleased(int key){
		if(key<0) return false;
		if(!this->keys.at(key).first && this->keys.at(key).second) return true;
		return false;
	}
	bool Input::isMouseJustPressed(int button){
		if(button<0) return false;
		if(this->mouseKeys.at(button).first && !this->mouseKeys.at(button).second) return true;
		return false;
	}
	bool Input::isMousePressed(int button){
		if(button<0) return false;
		if(this->mouseKeys.at(button).first) return true;
		return false;
	}
	bool Input::isMouseJustReleased(int button){
		if(button<0) return false;
		if(!this->mouseKeys.at(button).first && this->keys.at(button).second) return true;
		return false;
	}

	glm::vec2 Input::getScrollOffset(){
		return this->mouseScrollOffset;
	}

	glm::vec2 Input::getMouseOffset(){
		float xOffset = this->mousePos.x - this->prevMousePos.x;
		float yOffset = this->mousePos.y - this->prevMousePos.y;
		if(xOffset > 500 || yOffset > 500) return glm::vec2(0.f, 0.f);
		return glm::floor(glm::vec2(xOffset, yOffset));
	}

	//callbacks
	void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
		if(key<0) return;
		if(action == GLFW_PRESS){
			this->keys.at(key).first = true;
		}
		else if(action == GLFW_RELEASE){
			this->keys.at(key).first = false;
		}
	}
	void Input::cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
		this->prevMousePos = this->mousePos;
		this->mousePos = glm::vec2(xpos, ypos);
	}
	void Input::cursor_enter_callback(GLFWwindow* window, int entered){
		if(entered){
			double xpos = 0., ypos = 0.;
			if(this->mouseMode == MouseMode::Captured){
				glfwSetCursorPos(window, xpos, ypos);
			}
			else{
				glfwGetCursorPos(window, &xpos, &ypos);
			}
			this->prevMousePos = glm::vec2(xpos, ypos);
			this->mouseOnScreen = true;
		}
		else{
			this->mouseOnScreen = false;
		}
	}
	void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
		if(action == GLFW_PRESS){
			this->mouseKeys.at(button).first = true;
		}
		else if(action == GLFW_RELEASE){
			this->mouseKeys.at(button).first = false;
		}
	}
	void Input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
		this->mouseScrollOffset = glm::vec2(xoffset, yoffset);
	}
	void Input::window_focus_callback(GLFWwindow* window, int focused){
		if(focused){
			this->windowFocused = true;
		}
		else{
			this->windowFocused = false;
		}
	}
}