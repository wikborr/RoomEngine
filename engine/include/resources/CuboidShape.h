#pragma once
#include "resources/Resource.h"

namespace RoomEng{
	//resource class for collision objects
	class ROOMENGAPI CuboidShape: public Resource{
	public:
		glm::vec3 size = glm::vec3(0.f);

		void setSize(glm::vec3 size);
	};
}
