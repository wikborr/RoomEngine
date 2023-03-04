#include "roomObjects/RoomObject.h"

namespace RoomEng{
	void RoomObject::ready(){}
	void RoomObject::update(double delta){}
	void RoomObject::render(RoomObject* previouslyRenderedObject){}
	void RoomObject::objectOperation1(double delta){}
	int RoomObject::renderPriority(){
		return 3;
	}
	RoomObjectType RoomObject::roomObjectType(){
		return RoomObjectType::Object;
	}

	void RoomObject::objectOperation2(double delta){
		RoomObject* nextRoomObject = this->parent;
		if(nextRoomObject != nullptr){
			glm::mat4 transform = nextRoomObject->globalTransform;
			this->globalPosition = transform * glm::vec4(this->position, 1.0f);
			transform = glm::translate(transform, this->position);
			transform = glm::rotate(transform, glm::radians(this->rotationDegrees.x), glm::vec3(1.0f, 0.0f, 0.0f));
			transform = glm::rotate(transform, glm::radians(this->rotationDegrees.y), glm::vec3(0.0f, 1.0f, 0.0f));
			transform = glm::rotate(transform, glm::radians(this->rotationDegrees.z), glm::vec3(0.0f, 0.0f, 1.0f));
			transform = glm::scale(transform, this->scale);
			this->globalTransform = transform * this->transform;
		}
	}

	RoomObject::RoomObject(){
		this->name = "RoomObject";
	}

	RoomObject::~RoomObject(){
		if(this->shader != nullptr){
			ResourceManager::ins().unloadShader(this->shader);
		}
		if(this->parent != nullptr){
			this->parent->removeChild(this);
		}
		std::vector<RoomObject*> toDelete = this->children;
		for(auto i: toDelete){
			delete i;
		}
	}

	RoomObject* RoomObject::getChild(std::string name){
		if(!(this->children.empty())){
			for(const auto &i: this->children){
				if(i->name == name){
					return i;
				}
			}
		}
		return nullptr;
	}

	RoomObject* RoomObject::getParent(){
		return this->parent;
	}

	std::vector<RoomObject*> RoomObject::getAllChildren(){
		if(this->children.empty()){
			return std::vector<RoomObject*>();
		}
		std::vector<RoomObject*> descendants;
		for(const auto &i: this->children){
			descendants.push_back(i);
			std::vector<RoomObject*> i_vect = i->getAllChildren();
			descendants.insert(descendants.end(), i_vect.begin(), i_vect.end());
		}
		return descendants;
	}

	RoomObject* RoomObject::getRoot(){
		RoomObject* root = this->parent;
		while(root != nullptr) root = root->parent;
		return root;
	}

	RoomObject* RoomObject::removeChild(std::string name){
		RoomObject* childToRemove = nullptr;
		for(const auto& i: this->children){
			if(i->name == name){
				childToRemove = i;
			}
		}
		return this->removeChild(childToRemove);
	}

	RoomObject* RoomObject::removeChild(RoomObject* roomObject){
		this->children.erase(std::remove(this->children.begin(), this->children.end(), roomObject), this->children.end());
		roomObject->parent = nullptr;
		return roomObject;
	}

	glm::vec3 RoomObject::getGlobalPosition() const{
		return this->globalPosition;
	}
	glm::mat4 RoomObject::getGlobalTransform() const{
		return this->globalTransform;
	}
}