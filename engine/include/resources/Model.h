#pragma once
#include "resources/Resource.h"
#include "resources/Mesh.h"

namespace RoomEng{
	struct ROOMENGAPI MeshNode{
		std::vector<Mesh*> meshes;
		std::vector<MeshNode*> children;
		
		glm::mat4 transform = glm::mat4(1.f);
		glm::mat4 globalTransform = glm::mat4(1.f);
	};

	//resource class for 3d models
	class ROOMENGAPI Model: public Resource{
	public:
		Model* prototype = nullptr;
		MeshNode* meshRoot = nullptr;

		void createModel(MeshNode* root);
		void destroyModel();
		void destroyPrototype();
		Model* newCopy();
	private:
		void destroyMeshNodes(MeshNode*& meshNode, bool meshesToo = false);
		void copyMeshNodes(MeshNode*& meshNode, MeshNode*& oldMeshNode);
	};
}
