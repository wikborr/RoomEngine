#ifndef _ROOT_ROOM
#define _ROOT_ROOM
#include "roomObjects/RoomObject.h"
#include "Renderer.h"
#include "Physics.h"
#include "Input.h"

class RootRoom: public RoomEng::RoomObject{
public:
	RoomEng::Texture* cubeMap = nullptr;

	void ready() override;
	void update(double delta) override;
	void render(RoomObject* previouslyRenderedObject) override;
	int renderPriority() override;
};
#endif

int RootRoom::renderPriority(){
	return 1;
}

//function that is only executed once
void RootRoom::ready() {
	//loading skybox
	this->cubeMap = RoomEng::ResourceManager::ins().loadCubemap({
		"assets/textures/skybox/example_skybox/right.jpg",
		"assets/textures/skybox/example_skybox/left.jpg",
		"assets/textures/skybox/example_skybox/top.jpg",
		"assets/textures/skybox/example_skybox/bottom.jpg",
		"assets/textures/skybox/example_skybox/front.jpg",
		"assets/textures/skybox/example_skybox/back.jpg"
	});
}

//function that is executed every frame
void RootRoom::update(double delta){
	
	if(RoomEng::Input::ins().isKeyJustPressed(GLFW_KEY_ESCAPE)){
		RoomEng::Settings::ins().close();
	}
	if(RoomEng::Input::ins().isKeyJustPressed(GLFW_KEY_F)){
		RoomEng::Settings::ins().window.goFullscreen();
	}

	//std::cout<<"FPS: "<<static_cast<int>(1/deltaTime)<<std::endl;
}

//custom render function
void RootRoom::render(RoomObject* previouslyRenderedObject){
	RoomEng::Renderer::ins().drawSkybox(this->cubeMap);
}

