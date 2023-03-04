#pragma once
#include "Utils.h"
#include "FileReader.h"

#include "resources/Resource.h"
#include "resources/Texture.h"
#include "resources/Shader.h"
#include "resources/CuboidShape.h"
#include "resources/Model.h"
#include "resources/Mesh.h"


namespace RoomEng{
	//factory class that manages resources
	class ROOMENGAPI ResourceManager{
	public:
		//singleton method
		ResourceManager(){}
		static ResourceManager& ins();

		//resource pools
		std::vector<Texture*> texturePool;
		std::vector<Shader*> shaderPool;
		std::vector<CuboidShape*> cuboidPool;
		std::vector<Mesh*> meshPool;
		std::vector<Model*> modelPool;

		//methods
		void init();
		void destroy();

		//positive means it already exists, negative means it has to be created
		template <typename Res>
		int getPoolIndex(std::vector<Res*>& pool, std::string value);

		//resource generation methods
		//1 path for a 2d texture
		Texture* loadTexture(std::string path, unsigned filter = GL_LINEAR);
		//array of 6 paths for skybox
		Texture* loadCubemap(std::array<std::string,6> skyBoxPaths, unsigned filter = GL_LINEAR);
		Shader* loadShader(std::string fragmentPath = "shaders/default.frag", std::string vertexPath = "shaders/default.vert", std::string geometryPath = "");
		CuboidShape* loadCuboid(glm::vec3 size);
		Mesh* loadMesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<Material> textures, glm::vec3 color = glm::vec3(1.f));
		Model* loadModel(std::string path);

		void unloadTexture(Texture* texture);
		void unloadShader(Shader* shader);
		void unloadCuboid(CuboidShape* cuboid);
		void unloadMesh(Mesh* mesh);
		void unloadModel(Model* model);

	private:
		MeshNode* processNode(aiNode* node, const aiScene* scene, std::string path);
		Mesh* processMesh(aiMesh* assMesh, const aiScene* scene, std::string path);
	};
}