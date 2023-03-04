#include "roomObjects/RoomLightSource.h"

namespace RoomEng{
	RoomLightSource::RoomLightSource(){
		this->name = "RoomLightSource";
	}
	int RoomLightSource::renderPriority(){
		return 2;
	}
	RoomObjectType RoomLightSource::roomObjectType(){
		return RoomObjectType::LightSource;
	}

	void RoomLightSource::render(RoomObject* previouslyRenderedObject){
		Renderer::ins().setShaderProgram(nullptr); //setting the default shader
;		if(this->lightType == LightType::Directional){
			Renderer::ins().setDirectionalLight(this->dirVector, this->color);
		}
		else if(this->lightType == LightType::Point){
			Renderer::ins().setPointLight(this->getGlobalPosition(), this->pointExposition, this->color, this->whichUniform);
		}
		else if(this->lightType == LightType::Spot){
			Renderer::ins().setSpotLight(this->getGlobalPosition(), this->dirVector, this->spotAngle, this->color, this->whichUniform);
		}
	}
}