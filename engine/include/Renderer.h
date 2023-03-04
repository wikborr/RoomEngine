#pragma once
#include "Utils.h"
#include "ResourceManager.h"
#include "Scene.h"

namespace RoomEng{
	//class that contains global project settings
	class ROOMENGAPI Renderer{
	public:
		//singleton method
		Renderer(){}
		static Renderer& ins();
		
		Scene* currentScene = nullptr;
		Shader* defaultShader = nullptr;
		Shader* cubemapShader = nullptr;
		Model* cubemapModel = nullptr;

		glm::mat4 modelMatrix = glm::mat4(1.f);
		glm::mat4 viewMatrix = glm::mat4(1.f);
		glm::vec3 cameraPosition = glm::vec3(0.f);
		glm::mat4 perspectiveMatrix = glm::mat4(1.f);
		Shader* currentShader = nullptr;
		unsigned currentVAO = 0;

		void init(Scene* currentScene);
		void render();
		void destroy();

		void setDirectionalLight(glm::vec3 dirVector, glm::vec3 color = glm::vec3(1.f));
		void setSpotLight(glm::vec3 position, glm::vec3 direction, float spotAngle, glm::vec3 color = glm::vec3(1.f), int whichUniform = 1);
		void setPointLight(glm::vec3 position, float exposition, glm::vec3 color = glm::vec3(1.f), int whichUniform = 1);
		void unsetLights();
		void setModelMatrix(glm::mat4 modelMatrix);
		void setViewMatrix(glm::mat4 viewMatrix);
		void setPerspectiveMatrix(glm::mat4 perspectiveMatrix);
		void setCameraPosition(glm::vec3 cameraPos);
		void setShaderProgram(Shader* shaderProgram = nullptr);
		void setTexture(Texture* texture, int textureUnit = 0);
		void unsetTextures();
		void setVAO(unsigned VAO);
		void unsetVAO();
		void drawMesh(Mesh* mesh, Shader* shaderProgram = nullptr, glm::mat4 modelMatrix = glm::mat4(1.f));
		void drawModel(Model* model, Shader* shaderProgram  = nullptr);
		void drawSkybox(Texture* texture);
		
	private:
		void drawMeshNode(MeshNode* meshNode, Shader* shaderProgram  = nullptr);
		void textureSetter(Mesh* mesh);
	};
}