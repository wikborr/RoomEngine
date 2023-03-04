#include "ResourceManager.h"

namespace RoomEng{
	ResourceManager& ResourceManager::ins(){
		static ResourceManager instance;
		return instance;
	}

	void ResourceManager::init(){
		stbi_set_flip_vertically_on_load(false);
		this->texturePool.reserve(100);
		this->shaderPool.reserve(10);
		this->cuboidPool.reserve(50);
		this->meshPool.reserve(50);
		this->modelPool.reserve(50);

		for(auto& i: this->texturePool){
			i = new Texture;
		}
		for(auto& i: this->shaderPool){
			i = new Shader;
		}
		for(auto& i: this->cuboidPool){
			i = new CuboidShape;
		}
		for(auto& i: this->meshPool){
			i = new Mesh;
		}
		for(auto& i: this->modelPool){
			i = new Model;
		}
	}

	void ResourceManager::destroy(){
		for(auto& i: this->texturePool){
			delete i;
		}
		for(auto& i: this->shaderPool){
			delete i;
		}
		for(auto& i: this->cuboidPool){
			delete i;
		}
		for(auto& i : this->modelPool){
			i->destroyPrototype();
			delete i;
		}
		for(auto& i: this->meshPool){
			delete i;
		}
	}

	template <typename Res>
	int ResourceManager::getPoolIndex(std::vector<Res*>& pool, std::string stringID){
		for(int i = 0; i < pool.size(); i++){
			if(pool.at(i)->stringID == stringID){
				pool.at(i)->references+=1;
				return i;
			}
		}
		for(int i = 0; i < pool.size(); i--){
			if(pool.at(i)->references < 1){
				pool.at(i)->references+=1;
				pool.at(i)->stringID = stringID;
				return (-i)-1;
			}
		}
		Res* newRes = new Res;
		newRes->references+=1;
		newRes->stringID = stringID;
		pool.push_back(newRes);
		return -(pool.size() - 1)-1;
	}

	Texture* ResourceManager::loadTexture(std::string path, unsigned filter){
		int ind = this->getPoolIndex<Texture>(this->texturePool, path);
		if(ind > -1){
			return this->texturePool.at(ind);
		}
		spdlog::info("loading texture: "+path);

		Texture* texture = texturePool.at(-(ind + 1));
		int nrChannels = 0; 
		unsigned char* data = stbi_load(path.c_str(), &(texture->size.x), &(texture->size.y), &nrChannels, 4);
		if(!data){
			spdlog::error("texture not found: "+path);
		}
		texture->setTexture(data, GL_RGBA, GL_RGBA, filter);
		stbi_image_free(data);
		return texture;
	}
	Texture* ResourceManager::loadCubemap(std::array<std::string, 6> skyBoxPaths, unsigned filter) {
		std::string stringID = "";
		for(const auto& i : skyBoxPaths) stringID += i;
		int ind = this->getPoolIndex<Texture>(this->texturePool, stringID);
		if(ind > -1){
			return this->texturePool.at(ind);
		}
		spdlog::info("loading cubemap: " + skyBoxPaths.at(1));

		Texture* texture = texturePool.at(-(ind + 1));
		int nrChannels = 0;
		std::array<unsigned char*, 6> data;
		for(int i = 0; i < data.size(); i++){
			data.at(i) = stbi_load(skyBoxPaths.at(i).c_str(), &(texture->size.x), &(texture->size.y), &nrChannels, 4);
			if(!data.at(i)){
				spdlog::error("texture not found: " + skyBoxPaths.at(i));
			}
		}
		texture->setSkyBox(data, GL_RGBA, GL_RGBA, filter);
		for(auto& i : data){
			stbi_image_free(i);
		}
		return texture;
	}

	Shader* ResourceManager::loadShader(std::string fragmentPath, std::string vertexPath, std::string geometryPath){
		std::string stringID = fragmentPath+vertexPath+geometryPath;
		int ind = this->getPoolIndex<Shader>(this->shaderPool, stringID);
		if(ind > -1){
			return this->shaderPool.at(ind);
		}
		spdlog::info("loading shaders: "+vertexPath+" "+fragmentPath+" "+geometryPath);

		Shader* shader = shaderPool.at(-(ind + 1));
		std::string fragmentSrc = FileReader::loadAsString(fragmentPath);
		if(fragmentSrc == ""){
			spdlog::error("fragment shader not found: "+fragmentPath);
			return nullptr;
		}
		std::string vertexSrc = FileReader::loadAsString(vertexPath);
		if(vertexSrc == ""){
			spdlog::error("vertex shader not found: "+vertexPath);
			return nullptr;
		}
		std::string geometrySrc = FileReader::loadAsString(geometryPath);
		if(geometryPath != "" && geometrySrc == ""){
			spdlog::error("geometry shader not found: "+geometryPath);
		}
		shader->compileShaders(vertexSrc, fragmentSrc, geometrySrc);
		return shader;
	}

	CuboidShape* ResourceManager::loadCuboid(glm::vec3 size){
		std::ostringstream ss;
		ss<<size.x<<size.y<<size.z;
		std::string stringID(ss.str());
		int ind = this->getPoolIndex<CuboidShape>(this->cuboidPool, stringID);
		if(ind > -1){
			return this->cuboidPool.at(ind);
		}

		CuboidShape* cuboid = cuboidPool.at(-(ind + 1));
		cuboid->setSize(size);
		return cuboid;
	}

	Mesh* ResourceManager::loadMesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<Material> textures, glm::vec3 color){
		std::string meshString;
		for(auto& i: vertices) Utils::addBytesToString(meshString, &i);
		for(auto& i: indices) Utils::addBytesToString(meshString, &i);
		for(auto& i: textures) Utils::addBytesToString(meshString, &i);
		std::hash<std::string> stringHash;
		std::string stringID = std::to_string(stringHash(meshString));
		int ind = this->getPoolIndex<Mesh>(this->meshPool, stringID);
		if(ind > -1){
			return this->meshPool.at(ind);
		}

		Mesh* mesh = meshPool.at(-(ind + 1));
		mesh->createMesh(vertices, indices, textures, color);
		return mesh;
	}

	Model* ResourceManager::loadModel(std::string path){
		int ind = this->getPoolIndex<Model>(this->modelPool, path);
		if(ind > -1){
			return this->modelPool.at(ind)->newCopy();
		}
		spdlog::info("loading model: "+path);
		Model* model = modelPool.at(-(ind + 1));

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
			spdlog::error("assimp: model not found or incompatible: " + path);
			return nullptr;
		}
		MeshNode* rootMeshNode = this->processNode(scene->mRootNode, scene, path);
		model->createModel(rootMeshNode);
		return model->newCopy();
	}

	MeshNode* ResourceManager::processNode(aiNode* node, const aiScene* scene, std::string path) {
		MeshNode* meshNode = new MeshNode;
		meshNode->transform = Utils::aiMat2glmMat(node->mTransformation);
		for(int i = 0; i < node->mNumMeshes; i++){
			Mesh* newMesh = this->processMesh(scene->mMeshes[node->mMeshes[i]], scene, path);
			meshNode->meshes.push_back(newMesh);
		}
		for(int i = 0; i < node->mNumChildren; i++){
			MeshNode* newMeshNode = this->processNode(node->mChildren[i], scene, path);
			meshNode->children.push_back(newMeshNode);
		}
		return meshNode;
	}

	Mesh* ResourceManager::processMesh(aiMesh* assMesh, const aiScene* scene, std::string path){
		std::vector<Vertex> vertices;
		std::vector<unsigned> indices;
		std::vector<Material> textures;
		for(int i = 0; i < assMesh->mNumVertices; i++){
			Vertex vertex;
			vertex.position.x = assMesh->mVertices[i].x;
			vertex.position.y = assMesh->mVertices[i].y;
			vertex.position.z = assMesh->mVertices[i].z;
			vertex.normal.x = assMesh->mNormals[i].x;
			vertex.normal.y = assMesh->mNormals[i].y;
			vertex.normal.z = assMesh->mNormals[i].z;
			if(assMesh->HasTangentsAndBitangents()){
				vertex.tangent.x = assMesh->mTangents[i].x;
				vertex.tangent.y = assMesh->mTangents[i].y;
				vertex.tangent.z = assMesh->mTangents[i].z;
				vertex.bitangent.x = assMesh->mBitangents[i].x;
				vertex.bitangent.y = assMesh->mBitangents[i].y;
				vertex.bitangent.z = assMesh->mBitangents[i].z;
			}
			if (assMesh->mTextureCoords[0]){
				vertex.texCoords.x = assMesh->mTextureCoords[0][i].x;
				vertex.texCoords.y = assMesh->mTextureCoords[0][i].y;
			}
			else{
				vertex.texCoords = glm::vec2(0.f);
			}
			vertices.push_back(vertex);
		}
		for (int i = 0; i < assMesh->mNumFaces; i++){
			aiFace face = assMesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; j++){
				indices.push_back(face.mIndices[j]);
			}
		}
		if (assMesh->mMaterialIndex >= 0){
			std::string directory = path.substr(0, path.find_last_of('/') + 1);
			std::vector<std::pair<aiTextureType, TexType>> texPaths = {
				{aiTextureType_AMBIENT, TexType::Ambient},
				{aiTextureType_DIFFUSE, TexType::Diffuse},
				{aiTextureType_SPECULAR, TexType::Specular},
				{aiTextureType_DISPLACEMENT, TexType::Parallax},
				{aiTextureType_NORMALS, TexType::Normal},
				{aiTextureType_NONE, TexType::Albedo},
				{aiTextureType_METALNESS, TexType::Metallic},
				{aiTextureType_DIFFUSE_ROUGHNESS, TexType::Roughness},
				{aiTextureType_AMBIENT_OCCLUSION, TexType::AmbientOcclusion},
				{aiTextureType_UNKNOWN, TexType::AMR},
				{aiTextureType_BASE_COLOR, TexType::BaseColor},
			};
			aiMaterial* material = scene->mMaterials[assMesh->mMaterialIndex];
			for (auto& i : texPaths){
				for (int j = 0; j < material->GetTextureCount(i.first); j++){
					aiString str;
					material->GetTexture(i.first, j, &str);
					Material texture;
					texture.texture = this->loadTexture(directory + str.C_Str());
					texture.type = i.second;
					textures.push_back(texture);
				}
			}
		}
		return this->loadMesh(vertices, indices, textures);
	}


	void ResourceManager::unloadTexture(Texture* texture){
		for(auto& i: this->texturePool){
			if(texture == i){
				i->references -= 1;
				return;
			}
		}
	}
	void ResourceManager::unloadShader(Shader* shader){
		for(auto& i: this->shaderPool){
			if(shader == i){
				i->references -= 1;
				return;
			}
		}
	}
	void ResourceManager::unloadCuboid(CuboidShape* cuboid){
		for(auto& i: this->cuboidPool){
			if(cuboid == i){
				i->references -= 1;
				return;
			}
		}
	}
	void ResourceManager::unloadMesh(Mesh* mesh){
		for(auto& i: this->meshPool){
			if(mesh == i){
				i->references -= 1;
				if(i->references < 1){
					for(auto& j: i->textures){
						if(j.texture != nullptr)
							this->unloadTexture(j.texture);
					}
				}
				return;
			}
		}
	}
	void ResourceManager::unloadModel(Model* model){
		if(model->prototype == nullptr) return;
		Model* prototype = model->prototype;
		model->destroyModel();
		delete model;
		for(auto& i: this->modelPool){
			if(prototype == i){
				i->references -= 1;
				if(i->references < 1){
					i->destroyPrototype();
				}
				return;
			}
		}
	}
}