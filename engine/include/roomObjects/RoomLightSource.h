#pragma once
#include "roomObjects/RoomObject.h"
#include "Renderer.h"
#include "Physics.h"

namespace RoomEng{
	enum class LightType{
		Directional, //one vector, distance and position do not matter, for sunlight
		Point, //position and distance does matter (uses exposition), for lightbulbs
		Spot, //for a flashlight
	};

	//casts light
	class ROOMENGAPI RoomLightSource: public RoomObject{
	public:
		RoomLightSource();
		virtual int renderPriority() override;
		virtual RoomObjectType roomObjectType() override;
		virtual void render(RoomObject* previouslyRenderedObject) override;
		
		int whichUniform = 0;
		glm::vec3 color = glm::vec3(1.f);
		LightType lightType = LightType::Directional;
		glm::vec3 dirVector = glm::vec3(1.f, -0.5f, 1.f); //for directional light and spot light
		float pointExposition = 1.f; //only for point light
		float spotAngle = 15.f; //only for spot light
	};
}

