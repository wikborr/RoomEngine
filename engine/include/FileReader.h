#pragma once
#include "Utils.h"

namespace RoomEng{
	namespace FileReader{
		ROOMENGAPI std::string loadAsString(std::string path);
		ROOMENGAPI bool fileExists(std::string path);
	}
}