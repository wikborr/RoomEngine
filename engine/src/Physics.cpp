#include "Physics.h"

namespace RoomEng{
	Physics& Physics::ins(){
		static Physics instance;
		return instance;
	}
	
}