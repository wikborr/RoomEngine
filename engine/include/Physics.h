#pragma once
#include "Utils.h"
#include "FileReader.h"

namespace RoomEng{
	//class that contains global project settings
	class ROOMENGAPI Physics{
	public:
		//singleton method
		Physics(){}
		static Physics& ins();

		
	};
}