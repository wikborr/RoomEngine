#include "Renderer.h"
#include "../../src/MainCamera.hpp"

namespace RoomEng{
	Renderer& Renderer::ins(){
		static Renderer instance;
		return instance;
	}
	
	void Renderer::init(Scene* currentScene){
		//black screen for initializing the engine
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(Settings::ins().window.window);
		
		this->currentScene = currentScene;
		this->defaultShader = ResourceManager::ins().loadShader();
		this->cubemapShader = ResourceManager::ins().loadShader("shaders/skybox.frag", "shaders/skybox.vert");
		this->cubemapModel = ResourceManager::ins().loadModel("assets/models/cube.obj");
		this->currentShader = this->defaultShader;

		//enable depth buffer
		glEnable(GL_DEPTH_TEST);

		//enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//enable face culling
		//activate if framerate starts getting bad
		//glEnable(GL_CULL_FACE);

		//default matrices
		this->viewMatrix = glm::lookAt(glm::vec3(0.f, 0.f, 10.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		this->cameraPosition = glm::vec3(0.f, 0.f, 10.f);
		float aspectRatio = static_cast<float>(Settings::ins().window.screenWidth)/static_cast<float>(Settings::ins().window.screenHeight);
		this->perspectiveMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.f);
	}

	void Renderer::destroy(){
		if(this->defaultShader != nullptr){
			ResourceManager::ins().unloadShader(this->defaultShader);
		}
	}

	void Renderer::render(){
		//clearing the buffer
		glClearColor(Settings::ins().window.bgColor.r, Settings::ins().window.bgColor.g, Settings::ins().window.bgColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//resetting current properties
		this->currentShader = this->defaultShader;
		glActiveTexture(GL_TEXTURE0);

		//do every object's render function
		this->currentScene->doRenders();

		//buffer swap
		glfwSwapBuffers(Settings::ins().window.window);
	}

	void Renderer::setDirectionalLight(glm::vec3 dirVector, glm::vec3 color){
		glUniform3f(glGetUniformLocation(this->currentShader->programID, "dirLight.direction"), dirVector.x, dirVector.y, dirVector.z);
		glUniform3f(glGetUniformLocation(this->currentShader->programID, "dirLight.color"), color.r, color.g, color.b);
	}
	void Renderer::setSpotLight(glm::vec3 position, glm::vec3 direction, float spotAngle, glm::vec3 color, int whichUniform){
		std::string uniName = std::string("spotLight") + std::to_string(whichUniform);
		glUniform3f(glGetUniformLocation(this->currentShader->programID, std::string(uniName+".position").c_str()), position.x, position.y, position.z);
		glUniform3f(glGetUniformLocation(this->currentShader->programID, std::string(uniName + ".direction").c_str()), direction.x, direction.y, direction.z);
		glUniform1f(glGetUniformLocation(this->currentShader->programID, std::string(uniName + ".angle").c_str()), spotAngle);
		glUniform3f(glGetUniformLocation(this->currentShader->programID, std::string(uniName + ".color").c_str()), color.r, color.g, color.b);
	}
	void Renderer::setPointLight(glm::vec3 position, float exposition, glm::vec3 color, int whichUniform){
		std::string uniName = std::string("pointLight") + std::to_string(whichUniform);
		glUniform3f(glGetUniformLocation(this->currentShader->programID, std::string(uniName + ".position").c_str()), position.x, position.y, position.z);
		glUniform1f(glGetUniformLocation(this->currentShader->programID, std::string(uniName + ".exposition").c_str()), exposition);
		glUniform3f(glGetUniformLocation(this->currentShader->programID, std::string(uniName + ".color").c_str()), color.r, color.g, color.b);
	}

	void Renderer::unsetLights(){
		for(int i = 1; i < 4; i++){
			std::string uniName = std::string("pointLight") + std::to_string(i);
			glUniform1f(glGetUniformLocation(this->currentShader->programID, std::string(uniName + ".exposition").c_str()), 0.f);
		}
		glUniform1f(glGetUniformLocation(this->currentShader->programID, std::string("spotLight1.angle").c_str()), 0.f);
	}

	void Renderer::setModelMatrix(glm::mat4 modelMatrix){
		this->modelMatrix = modelMatrix;
	}

	void Renderer::setViewMatrix(glm::mat4 viewMatrix){
		this->viewMatrix = viewMatrix;
	}

	void Renderer::setPerspectiveMatrix(glm::mat4 perspectiveMatrix){
		this->perspectiveMatrix = perspectiveMatrix;
	}

	void Renderer::setCameraPosition(glm::vec3 cameraPos){
		this->cameraPosition = cameraPos;
	}

	void Renderer::setShaderProgram(Shader* shaderProgram){
		if(shaderProgram != nullptr){
			this->currentShader = shaderProgram;
		}
		else{
			this->currentShader = this->defaultShader;
		}
		glUseProgram(this->currentShader->programID);
	}

	void Renderer::setTexture(Texture* texture, int textureUnit){
		if(texture != nullptr){
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_2D, texture->textureID);
			switch(textureUnit){
			case 0:
				glUniform1i(glGetUniformLocation(this->currentShader->programID, "texAlbedoActive"), true);
				break;
			case 1:
				glUniform1i(glGetUniformLocation(this->currentShader->programID, "texMetallicActive"), true);
				break;
			case 2:
				glUniform1i(glGetUniformLocation(this->currentShader->programID, "texRoughnessActive"), true);
				break;
			case 3:
				glUniform1i(glGetUniformLocation(this->currentShader->programID, "texAOActive"), true);
				break;
			case 4:
				glUniform1i(glGetUniformLocation(this->currentShader->programID, "texNormalActive"), true);
				break;
			case 5:
				glUniform1i(glGetUniformLocation(this->currentShader->programID, "texParallaxActive"), true);
				break;
			case 6:
				glUniform1i(glGetUniformLocation(this->currentShader->programID, "texBaseColorActive"), true);
				break;
			case 7:
				glUniform1i(glGetUniformLocation(this->currentShader->programID, "texAMRActive"), true);
				break;
			}
		}
	}

	void Renderer::unsetTextures(){
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(this->currentShader->programID, "texAlbedoActive"), false);
		glUniform1i(glGetUniformLocation(this->currentShader->programID, "texMetallicActive"), false);
		glUniform1i(glGetUniformLocation(this->currentShader->programID, "texRoughnessActive"), false);
		glUniform1i(glGetUniformLocation(this->currentShader->programID, "texAOActive"), false);
		glUniform1i(glGetUniformLocation(this->currentShader->programID, "texNormalActive"), false);
		glUniform1i(glGetUniformLocation(this->currentShader->programID, "texParallaxActive"), false);
		glUniform1i(glGetUniformLocation(this->currentShader->programID, "texBaseColorActive"), false);
		glUniform1i(glGetUniformLocation(this->currentShader->programID, "texAMRActive"), false);
	}

	void Renderer::setVAO(unsigned VAO){
		this->currentVAO = VAO;
		glBindVertexArray(VAO);
	}

	void Renderer::unsetVAO(){
		glBindVertexArray(0);
	}

	void Renderer::textureSetter(Mesh* mesh){
		for(int i = 0; i < mesh->textures.size(); i++){
			if(mesh->textures.at(i).type == TexType::Albedo || mesh->textures.at(i).type == TexType::Diffuse) this->setTexture(mesh->textures.at(i).texture, 0);
			if(mesh->textures.at(i).type == TexType::Metallic) this->setTexture(mesh->textures.at(i).texture, 1);
			if(mesh->textures.at(i).type == TexType::Roughness) this->setTexture(mesh->textures.at(i).texture, 2);
			if(mesh->textures.at(i).type == TexType::AmbientOcclusion) this->setTexture(mesh->textures.at(i).texture, 3);
			if(mesh->textures.at(i).type == TexType::Normal) this->setTexture(mesh->textures.at(i).texture, 4);
			if(mesh->textures.at(i).type == TexType::Parallax) this->setTexture(mesh->textures.at(i).texture, 5);
			if(mesh->textures.at(i).type == TexType::BaseColor) this->setTexture(mesh->textures.at(i).texture, 6);
			if(mesh->textures.at(i).type == TexType::AMR) this->setTexture(mesh->textures.at(i).texture, 7);
		}
	}

	void Renderer::drawMesh(Mesh* mesh, Shader* shaderProgram, glm::mat4 modelMatrix){
		if(shaderProgram != nullptr) this->currentShader = shaderProgram;
		if(mesh != nullptr && mesh->VAO != 0){
			this->modelMatrix = modelMatrix;
			this->setShaderProgram(this->currentShader);
			this->textureSetter(mesh);
			this->setVAO(mesh->VAO);
			glm::mat4 modelM = this->modelMatrix;
			glm::mat4 viewM = this->viewMatrix;
			glm::mat4 perspectiveM = this->perspectiveMatrix;
			glUniform3f(glGetUniformLocation(this->currentShader->programID, "camPos"), this->cameraPosition.x, this->cameraPosition.y, this->cameraPosition.z);
			glUniformMatrix4fv(glGetUniformLocation(this->currentShader->programID, "modelM"), 1, GL_FALSE, glm::value_ptr(modelM));
			glUniformMatrix4fv(glGetUniformLocation(this->currentShader->programID, "viewM"), 1, GL_FALSE, glm::value_ptr(viewM*modelM));
			glm::mat4 finalTransform = perspectiveM*viewM*modelM;
			finalTransform = this->perspectiveMatrix * this->viewMatrix * this->modelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(this->currentShader->programID, "transformation"), 1, GL_FALSE, glm::value_ptr(finalTransform));
			glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);

			//check for errors
			GLenum err;
			while ((err = glGetError()) != GL_NO_ERROR){
				spdlog::error("OpenGL error: " + std::to_string(err));
			}
			
			//resetting current properties
			this->unsetVAO();
			this->setShaderProgram(this->defaultShader);
			this->unsetTextures();
			this->unsetLights();
		}
	}

	void Renderer::drawSkybox(Texture* skyboxTexture) {
		if(skyboxTexture == nullptr) return;
		this->setShaderProgram(this->cubemapShader);
		this->setVAO(this->cubemapModel->meshRoot->children.at(0)->meshes.at(0)->VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture->textureID);
		glm::mat4 transformation = this->perspectiveMatrix * this->viewMatrix * glm::translate(glm::mat4(1.f), this->cameraPosition);
		glm::vec3 cameraPos = glm::vec3();
		glUniformMatrix4fv(glGetUniformLocation(this->currentShader->programID, "transformation"), 1, GL_FALSE, glm::value_ptr(transformation));
		glDrawElements(GL_TRIANGLES, this->cubemapModel->meshRoot->children.at(0)->meshes.at(0)->indices.size(), GL_UNSIGNED_INT, 0);
		this->setShaderProgram(this->defaultShader);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::drawModel(Model* model, Shader* shaderProgram){
		if(shaderProgram == nullptr) shaderProgram = this->currentShader;
		if(model != nullptr && model->meshRoot != nullptr){
			this->drawMeshNode(model->meshRoot, shaderProgram);
		}
	}

	void Renderer::drawMeshNode(MeshNode* meshNode, Shader* shaderProgram){
		for(auto& i : meshNode->meshes){
			this->drawMesh(i, shaderProgram, meshNode->globalTransform);
		}
		for(auto i: meshNode->children){
			this->drawMeshNode(i, shaderProgram);
		}
	}
}