#include "Core.h"

//add room object headers here
#include "RootRoom.hpp"
#include "MainCamera.hpp"
#include "ExampleRoom.hpp"

int main(){
	//add room objects and create a scene tree here
	RoomEng::RoomObject* rootRoom = new RootRoom;
	rootRoom->name = "RootRoom";
	rootRoom->addChild<MainCamera>("mainCamera");
	rootRoom->addChild<ExampleRoom>("exampleRoom");

	RoomEng::Scene* scene = new RoomEng::Scene(rootRoom);
	RoomEng::Core core;
	core.init(scene, "The Room", 960, 540, true); //window title, width, height and isWindowed
	return 0;
}