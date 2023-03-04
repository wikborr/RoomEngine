#ifndef _SUNLIGHT
#define _SUNLIGHT
#include "roomObjects/RoomLightSource.h"

class SunLight: public RoomEng::RoomLightSource{
public:

	void ready() override;
};
#endif

void SunLight::ready(){
	this->dirVector = glm::vec3(0.f, -1.f, -1.f);
}