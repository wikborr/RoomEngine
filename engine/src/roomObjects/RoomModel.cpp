#include "roomObjects/RoomModel.h"

namespace RoomEng{
	RoomModel::RoomModel(){
		this->name = "RoomModel";
	}

	RoomObjectType RoomModel::roomObjectType(){
		return RoomObjectType::Model;
	}

	RoomModel::~RoomModel(){
		if(this->model != nullptr){
			ResourceManager::ins().unloadModel(this->model);
		}
	}

	void RoomModel::objectOperation2(double delta){
		RoomObject::objectOperation2(delta);
		if(this->model != nullptr && this->model->meshRoot != nullptr){
			this->updateMeshNodeTransform(this->model->meshRoot, this->getGlobalTransform());
		}
	}

	void RoomModel::render(RoomObject* previouslyRenderedObject){
		Renderer::ins().drawModel(this->model, this->shader);
	}

	void RoomModel::updateMeshNodeTransform(MeshNode*& meshNode, glm::mat4 parentTransform){
		glm::mat4 transform = parentTransform;
		meshNode->globalTransform = transform * meshNode->transform;
		for(auto& i: meshNode->children){
			this->updateMeshNodeTransform(i, meshNode->globalTransform);
		}
	}
}