#pragma once
#include "Utils.h"

namespace RoomEng{
	//abstract class for resource objects
	class ROOMENGAPI Resource{
	public:
		int references = 0;
		//path or hash of data
		std::string stringID = ""; 
	};
}
