#pragma once
#include "Utils.h"
#include "Settings.h"

namespace RoomEng{
	enum class MouseMode{
		Visible,
		Hidden,
		Captured,
	};

	//class that contains global project settings
	class ROOMENGAPI Input{
	public:
		//singleton method
		Input(){}
		static Input& ins();

		MouseMode mouseMode = MouseMode::Visible;
		std::array<std::pair<bool, bool>, 512> keys;
		std::array<std::pair<bool, bool>, 16> mouseKeys;
		glm::vec2 mousePos = glm::vec2(0.0f, 0.0f);
		glm::vec2 prevMousePos = glm::vec2(0.0f, 0.0f);
		glm::vec2 mouseScrollOffset = glm::vec2(0.0f, 0.0f);
		bool mouseOnScreen = false;
		bool windowFocused = false;

		void init();
		void removeJustKeys();
		void changeMouseMode(MouseMode newMouseMode = MouseMode::Visible);
		glm::vec2 getMouseOffset();
		glm::vec2 getScrollOffset();

		bool isKeyJustPressed(int key);
		bool isKeyPressed(int key);
		bool isKeyJustReleased(int key);
		bool isMouseJustPressed(int button);
		bool isMousePressed(int button);
		bool isMouseJustReleased(int button);

		//callback functions
		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		void cursor_enter_callback(GLFWwindow* window, int entered);
		void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		void window_focus_callback(GLFWwindow* window, int focused);
	};
}