#pragma once
#include "resources/Resource.h"
#include "resources/Texture.h"

namespace RoomEng{
	//texture type
	enum class TexType{
		BaseColor,
		Diffuse,
		Specular,
		Normal,
		Parallax, //or other displacement maps
		Albedo,
		Metallic,
		Roughness,
		AmbientOcclusion,
		AMR, //Metallic, Roughness and Ambient Occlusion
		Ambient,
		Custom
	};

	struct ROOMENGAPI Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};

	struct ROOMENGAPI Material{
		Texture* texture = nullptr;
		TexType type = TexType::Diffuse;
	};

	//resource class for 3d meshes
	class ROOMENGAPI Mesh: public Resource{
	public:
		Mesh();
		~Mesh();
		unsigned VAO;
		std::vector<Material> textures;
		std::vector<unsigned> indices;
		glm::vec3 color = glm::vec3(1.f);
		
		void createMesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<Material> textures, glm::vec3 color = glm::vec3(1.f));
		void addTextureMap(Material textureMap);
		void addTextureMap(Texture* texture, TexType texType);
	};
}
