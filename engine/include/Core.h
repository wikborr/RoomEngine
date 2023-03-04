#pragma once
#include "Utils.h"
#include "Scene.h"

namespace RoomEng{
	//class that starts the engine
	class ROOMENGAPI Core{
	public:
		void init(Scene* scene, std::string projectName = "The Room", int width = 1280, int height = 720, bool windowed = true);
	private:
		Scene* scene = nullptr;
	};
}
