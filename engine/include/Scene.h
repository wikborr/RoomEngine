#pragma once
#include "Utils.h"
#include "roomObjects/RoomObject.h"

namespace RoomEng{
	//class that manages room objects
	class ROOMENGAPI Scene{
	public:
		//attributes
		RoomObject* root = nullptr;

		//methods
		Scene(RoomObject* root);
		~Scene();
		void changeScene(RoomObject* newRoot);

		void doReadies();
		void doObjectOperations1(double delta = 0.0);
		void doUpdates(double delta = 0.0);
		void doObjectOperations2(double delta = 0.0);
		void doRenders();

		std::vector<RoomObject*> getAllRoomObjects();
	
	private:
		std::vector<RoomObject*> allRoomObjects;
	};
}