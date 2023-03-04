#pragma once
#include "Utils.h"
#include "FileReader.h"
#include "ResourceManager.h"
#include "Settings.h"
#include "Input.h"

namespace RoomEng{
	enum class RoomObjectType{
		Object,
		Model,
		Camera,
		CollisionBody,
		CollisionArea,
		UIObject,
		Sprite,
		LightSource,
	};

	//base class for every in-game object
	class ROOMENGAPI RoomObject{
	public:
		//constructor and destructor
		RoomObject();
		virtual ~RoomObject();

		//attributes
		RoomObject* parent = nullptr;
		std::vector<RoomObject*> children;
		std::string name = "RoomObject";

		//local attributes
		glm::vec3 position = glm::vec3(0.0);
		glm::vec3 rotationDegrees = glm::vec3(0.0);
		glm::vec3 scale = glm::vec3(1.0);
		glm::mat4 transform = glm::mat4(1.0);

		bool visible = true;
		//nullptr means it's using the default shader
		Shader* shader = nullptr; 

		//virtual methods
		//called one time at the start of its lifetime
		virtual void ready();
		//called every frame for logic operations
		virtual void update(double delta);
		//called every frame to render, classes other than the base RoomObject already implement it
		virtual void render(RoomObject* previouslyRenderedObject);
		//for rendering order, you can override if you want
		virtual int renderPriority();
		//type of the object
		virtual RoomObjectType roomObjectType();
		
		//internal, avoid overriding it (called before the update method)
		virtual void objectOperation1(double delta);
		//also internal, avoid overriding it too (called after the update method)
		virtual void objectOperation2(double delta);

		//normal methods
		RoomObject* getChild(std::string name);
		RoomObject* getParent();
		std::vector<RoomObject*> getAllChildren();
		RoomObject* getRoot();
		//doesn't delete, only removes from the parent
		RoomObject* removeChild(std::string name);
		//doesn't delete, only removes from the parent
		RoomObject* removeChild(RoomObject* roomObject);

		glm::vec3 getGlobalPosition() const;
		glm::mat4 getGlobalTransform() const;

		template <typename T>
		T* getChild(std::string name){
			return static_cast<T*>(getChild(name));
		}
		template <typename T>
		T* getParent(){
			return static_cast<T*>(this->parent);
		}
		template <typename T>
		T* addChild(std::string name){
			RoomObject* newRoomObject = new T;
			newRoomObject->parent = this;
			this->children.push_back(newRoomObject);
			newRoomObject->name = name;
			return static_cast<T*>(newRoomObject);
		}
		template <typename T>
		T* addChild(T* roomObject){
			RoomObject* newRoomObject = static_cast<RoomObject*>(roomObject);
			newRoomObject->parent = this;
			this->children.push_back(newRoomObject);
			return static_cast<T*>(newRoomObject);
		}

	private:
		//global attributes
		glm::vec3 globalPosition = glm::vec3(0.0);
		glm::mat4 globalTransform = glm::mat4(1.0);
	};
}