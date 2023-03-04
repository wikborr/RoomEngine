#ifndef _EXAMPLE_ROOM
#define _EXAMPLE_ROOM
#include "roomObjects/RoomModel.h"

class ExampleRoom: public RoomEng::RoomModel{
public:
	
	void ready() override;
	void update(double delta) override;
	void render(RoomObject* previouslyRenderedObject) override;
};
#endif

void ExampleRoom::ready(){
	this->model = RoomEng::ResourceManager::ins().loadModel("assets/models/modern_interior_design/scene.gltf");
	this->scale *= 0.03;
}

void ExampleRoom::update(double delta){
	
}

void ExampleRoom::render(RoomObject* previouslyRenderedObject){

	RoomEng::RoomModel::render(previouslyRenderedObject);
}