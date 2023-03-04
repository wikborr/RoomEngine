#include "resources/Mesh.h"

namespace RoomEng{
	Mesh::Mesh(){
		glGenVertexArrays(1, &this->VAO);
	}

	Mesh::~Mesh(){
		glDeleteVertexArrays(1, &this->VAO);
	}

	void Mesh::createMesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<Material> textures, glm::vec3 color){
		if(vertices.size() == 0){
			this->VAO = 0;
			return;
		}
		this->color = color;
		this->textures = textures;
		this->indices = indices;
		glBindVertexArray(this->VAO);

		unsigned VBO, EBO;
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
	
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);  

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		//vertex positions
		glEnableVertexAttribArray(0);	
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		//vertex normals
		glEnableVertexAttribArray(1);	
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		//vertex texture coords
		glEnableVertexAttribArray(2);	
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
		//vertex tangets
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
		//vertex bitangents
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

		glBindVertexArray(0);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	void Mesh::addTextureMap(Material textureMap){
		std::vector<Material> newTextures;
		for(auto& i : this->textures){
			if(i.type != textureMap.type){
				newTextures.push_back(i);
			}
		}
		this->textures = newTextures;
		this->textures.push_back(textureMap);
	}

	void Mesh::addTextureMap(Texture* texture, TexType texType){
		Material mat;
		mat.texture = texture;
		mat.type = texType;
		this->addTextureMap(mat);
	}
}