#ifndef _MAIN_CAMERA
#define _MAIN_CAMERA
#include "roomObjects/RoomCamera.h"

//standard camera
class MainCamera: public RoomEng::RoomCamera{
public:
	bool cursorCaptured = true;
	float mouseSensitivity = 60.f;
	float velocity = 5.f;
	
	void ready() override;
	void update(double delta) override;
};
#endif

void MainCamera::ready(){
	RoomEng::Input::ins().changeMouseMode(RoomEng::MouseMode::Captured);
	this->position.y = 4;
	this->position.z = 20;
	this->far = 1000;
}

void MainCamera::update(double delta){
	//checking mouse mode
	if(RoomEng::Input::ins().isMouseJustPressed(GLFW_MOUSE_BUTTON_1)){
		if(this->cursorCaptured){
			RoomEng::Input::ins().changeMouseMode(RoomEng::MouseMode::Visible);
			this->cursorCaptured = false;
		}
		else{
			RoomEng::Input::ins().changeMouseMode(RoomEng::MouseMode::Captured);
			this->cursorCaptured = true;
		}
	}
	//looking around with camera
	if(this->cursorCaptured){
		this->cameraRotation.y += RoomEng::Input::ins().getMouseOffset().x * delta * mouseSensitivity;
		this->cameraRotation.x += RoomEng::Input::ins().getMouseOffset().y * delta * mouseSensitivity;
	}
	if(RoomEng::Input::ins().isKeyPressed(GLFW_KEY_Q)){
		this->cameraRotation.z -= delta*mouseSensitivity;
	}
	else if(RoomEng::Input::ins().isKeyPressed(GLFW_KEY_E)){
		this->cameraRotation.z += delta*mouseSensitivity;
	}	
	//moving camera
	float curVelocity = this->velocity;
	glm::vec3 finalVelocity = glm::vec3(0.f);
	if(RoomEng::Input::ins().isKeyPressed(GLFW_KEY_LEFT_SHIFT)){
		curVelocity *= 10;
	}
	if(RoomEng::Input::ins().isKeyPressed(GLFW_KEY_W)){
		finalVelocity.z -= delta*curVelocity;
	}
	else if(RoomEng::Input::ins().isKeyPressed(GLFW_KEY_S)){
		finalVelocity.z += delta*curVelocity;
	}
	if(RoomEng::Input::ins().isKeyPressed(GLFW_KEY_A)){
		finalVelocity.x -= delta*curVelocity;
	}
	else if(RoomEng::Input::ins().isKeyPressed(GLFW_KEY_D)){
		finalVelocity.x += delta*curVelocity;
	}
	if(RoomEng::Input::ins().isKeyPressed(GLFW_KEY_SPACE)){
		finalVelocity.y += delta*curVelocity;
	}
	else if(RoomEng::Input::ins().isKeyPressed(GLFW_KEY_LEFT_CONTROL)){
		finalVelocity.y -= delta*curVelocity;
	}
	
	this->move(finalVelocity, true);
}

