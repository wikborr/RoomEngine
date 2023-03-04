#include "roomObjects/RoomCamera.h"

namespace RoomEng{
	RoomCamera::RoomCamera(){
		this->name = "RoomCamera";
		this->smoothedPosition = this->getGlobalPosition();
	}
	int RoomCamera::renderPriority(){
		return 0;
	}
	RoomObjectType RoomCamera::roomObjectType(){
		return RoomObjectType::Camera;
	}

	void RoomCamera::objectOperation1(double delta){
		RoomObject::objectOperation1(delta);
		if (this->activated < 1){
			this->cameraRotation = glm::vec3(0.f);
			this->activated += 1;
			this->smoothedPosition = this->getGlobalPosition();
		}
		if(!this->current) return;

		if (this->cameraRotation.x > 89.9f){
			this->cameraRotation.x = 89.9f;
		}
		if(this->cameraRotation.x < -89.9f){
			this->cameraRotation.x = -89.9f;
		}		

		glm::vec3 basePosition = this->smoothedPosition;
		glm::vec3 targetPosition = basePosition + glm::vec3(0.f, 0.f, -1.f);
		glm::vec3 upVector = glm::vec3(0.f, 1.f, 0.f);
		glm::vec3 rightVector = glm::vec3(0.f);
		if(this->rotatesWithParent){
			targetPosition = this->getGlobalPosition() - this->position;
			float distance = glm::distance(basePosition, targetPosition);
			basePosition -= targetPosition;
			basePosition = glm::vec3(
				-glm::sin(glm::radians(this->cameraRotation.y))*glm::cos(glm::radians(this->cameraRotation.x)),
				glm::sin(glm::radians(this->cameraRotation.x)),
				glm::cos(glm::radians(this->cameraRotation.y))*glm::cos(glm::radians(this->cameraRotation.x))
			) * distance;
			rightVector = glm::cross(basePosition, upVector);
			upVector = glm::normalize(glm::cross(rightVector, basePosition));
			upVector = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1.f), -glm::radians(this->cameraRotation.z), glm::normalize(basePosition))*glm::vec4(upVector, 1.f)));
			basePosition += targetPosition;
		}
		else{
			targetPosition = glm::vec3(
				glm::sin(glm::radians(this->cameraRotation.y))*glm::cos(glm::radians(this->cameraRotation.x)),
				-glm::sin(glm::radians(this->cameraRotation.x)),
				-glm::cos(glm::radians(this->cameraRotation.y))*glm::cos(glm::radians(this->cameraRotation.x))
			);
			rightVector = glm::cross(targetPosition, upVector);
			upVector = glm::normalize(glm::cross(rightVector, targetPosition));
			upVector = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1.f), -glm::radians(this->cameraRotation.z), glm::normalize(-targetPosition))*glm::vec4(upVector, 1.f)));
			targetPosition += basePosition;
		}
		this->basePosition = basePosition;
		this->targetPosition = targetPosition;
		this->dirVector = glm::normalize(basePosition - targetPosition);
		this->rightVector = glm::normalize(rightVector);
		this->upVector = glm::normalize(upVector);
	}

	void RoomCamera::objectOperation2(double delta){
		RoomObject::objectOperation2(delta);
		if(this->current && this->smoothingEnabled){
			glm::vec3 diff = glm::abs(this->smoothedPosition - this->getGlobalPosition());
			if(diff.x > 0.1 || diff.y > 0.1 || diff.z > 0.1){
				glm::vec3 smoothDelta = (this->getGlobalPosition() - this->smoothedPosition) * static_cast<float>(delta) * this->smoothingSpeed;
				this->smoothedPosition += smoothDelta;
			}
		}
		else if(this->current){
			this->smoothedPosition = this->getGlobalPosition();
		}
	}

	void RoomCamera::render(RoomObject* previouslyRenderedObject){
		if(!this->current) return;
		//perspective
		float aspectRatio = static_cast<float>(Settings::ins().window.screenWidth)/Settings::ins().window.screenHeight;
		Renderer::ins().setPerspectiveMatrix(glm::perspective(glm::radians(fov/2), aspectRatio, this->near, this->far));

		//view
		Renderer::ins().setViewMatrix(glm::lookAt(this->basePosition, this->targetPosition, this->upVector));

	    //pos
		Renderer::ins().setCameraPosition(this->basePosition);
	}

	void RoomCamera::move(glm::vec3 velocity, bool flying){
		if(!this->rotatesWithParent){
			position += velocity.x * this->rightVector;
			position += velocity.y * glm::vec3(0.f, 1.f, 0.f);
			if(flying){
				position += velocity.z * this->dirVector;
			}
			else{
				position += velocity.z * glm::cross(rightVector, glm::vec3(0.f, 1.f, 0.f));
			}
		}
		else{
			position.z -= velocity.z;
			if(position.z < 0.f) position.z = 0.f;
		}
	}
}