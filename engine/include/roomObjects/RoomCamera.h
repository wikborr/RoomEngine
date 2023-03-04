#pragma once
#include "roomObjects/RoomObject.h"
#include "Renderer.h"
#include "Physics.h"

namespace RoomEng{
	//base room object dedicated to rendering models 
	class ROOMENGAPI RoomCamera: public RoomObject{
	public:
		RoomCamera();
		virtual void objectOperation1(double delta) override;
		virtual void objectOperation2(double delta) override;
		virtual void render(RoomObject* previouslyRenderedObject) override;
		virtual int renderPriority() override;
		virtual RoomObjectType roomObjectType() override;

		//is it the current camera
		bool current = true;
		//pitch, yaw, roll
		glm::vec3 cameraRotation = glm::vec3(0.f);
		//field of view
		float fov = 90;
		//decides if it's in the look at the object mode or not
		bool rotatesWithParent = false;
		float near = 0.1f;
		float far = 100.f;
		bool smoothingEnabled = false;
		float smoothingSpeed = 5.f;

		void move(glm::vec3 velocity, bool flying = false);

	private:
		int activated = -1;
		glm::vec3 smoothedPosition = glm::vec3(0.f);
		glm::vec3 rightVector =glm::vec3(1.f, 0.f, 0.f);
		glm::vec3 upVector = glm::vec3(0.f, 1.f, 0.f);
		glm::vec3 dirVector =  glm::vec3(0.f, 0.f, -1.f);
		glm::vec3 basePosition = glm::vec3(0.f);
		glm::vec3 targetPosition = glm::vec3(0.f);
	};
}