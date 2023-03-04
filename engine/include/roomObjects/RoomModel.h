#pragma once
#include "roomObjects/RoomObject.h"
#include "Renderer.h"
#include "Physics.h"

namespace RoomEng{
	//base room object dedicated to rendering models 
	class ROOMENGAPI RoomModel: public RoomObject{
	public:
		RoomModel();
		~RoomModel();
		virtual void objectOperation2(double delta) override;
		virtual void render(RoomObject* previouslyRenderedObject) override;
		virtual RoomObjectType roomObjectType() override;

		Model* model = nullptr;
	private:
		void updateMeshNodeTransform(MeshNode*& meshNode, glm::mat4 parentTransform);
	};
}