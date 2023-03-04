#include "resources/Model.h"
#include "ResourceManager.h"

namespace RoomEng{
	void Model::createModel(MeshNode* root){
		this->meshRoot = root;
	}

	void Model::destroyModel(){
		this->prototype = nullptr;
		this->destroyMeshNodes(this->meshRoot);
	}

	void Model::destroyPrototype(){
		this->destroyMeshNodes(this->meshRoot, true);
	}

	void Model::destroyMeshNodes(MeshNode*& meshNode, bool meshesToo){
		if(meshNode == nullptr) return;
		for(auto& i: meshNode->children){
			this->destroyMeshNodes(i, meshesToo);
		}
		if(meshesToo){
			for(auto& i : meshNode->meshes){
				ResourceManager::ins().unloadMesh(i);
			}
		}
		delete meshNode;
		meshNode = nullptr;
	}

	void Model::copyMeshNodes(MeshNode*& newMeshNode, MeshNode*& oldMeshNode){
		if(oldMeshNode == nullptr) return;
		newMeshNode = new MeshNode(*oldMeshNode);
		for(int i = 0; i < oldMeshNode->children.size(); i++){
			this->copyMeshNodes(newMeshNode->children.at(i), oldMeshNode->children.at(i));
		}
	}

	Model* Model::newCopy(){
		Model* model = new Model;
		model->prototype = this;
		model->meshRoot = nullptr;
		this->copyMeshNodes(model->meshRoot, this->meshRoot);
		return model;
	}
}