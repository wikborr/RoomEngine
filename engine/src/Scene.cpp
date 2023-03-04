#include "Scene.h"

namespace RoomEng{
	Scene::Scene(RoomObject* root){
		this->root = root;
	}

	Scene::~Scene(){
		if(this->root != nullptr){
			delete root;
			this->root = nullptr;
		}
	}

	void Scene::changeScene(RoomObject* newRoot){
		this->~Scene();
		this->root = newRoot;
		this->doReadies();
	}

	void Scene::doReadies(){
		if(this->root != nullptr){
			this->getAllRoomObjects();
			std::vector<RoomObject*> roReversed = this->allRoomObjects;
			std::reverse(roReversed.begin(), roReversed.end());
			for (const auto &i: roReversed){
				i->ready();
			}
		}
	}

	void Scene::doObjectOperations1(double delta){
		if(this->root != nullptr){
			for (const auto &i: this->allRoomObjects){
				i->objectOperation1(delta);
			}
		}
	}

	void Scene::doUpdates(double delta){
		if(this->root != nullptr){
			this->getAllRoomObjects();
			for (const auto &i: this->allRoomObjects){
				i->update(delta);
			}
		}
	}

	void Scene::doObjectOperations2(double delta){
		if(this->root != nullptr){
			this->getAllRoomObjects();
			for (const auto &i: this->allRoomObjects){
				i->objectOperation2(delta);
			}
		}
	}

	void Scene::doRenders(){
		if(this->root != nullptr){
			RoomObject* prevRender = nullptr;
			for(int i = 0; i < 4; i++){
				for (const auto &j: this->allRoomObjects){
					if(j->renderPriority() == i && j->visible){
						j->render(prevRender);
						prevRender = j;
					}
				}
			}
		}
	}

	std::vector<RoomObject*> Scene::getAllRoomObjects(){
		if(this->root != nullptr){
			std::vector<RoomObject*> allLeaves{this->root};
			std::vector<RoomObject*> childLeaves = this->root->getAllChildren();
			allLeaves.insert(allLeaves.end(), childLeaves.begin(), childLeaves.end());
			this->allRoomObjects = allLeaves;
			return this->allRoomObjects;
		}
		return std::vector<RoomObject*>();
	}
}