#pragma once
#include "resources/Resource.h"

namespace RoomEng{
	enum class TextureTarget{
		Texture2D,
		Cubemap,
	};

	//resource class for textures
	class ROOMENGAPI Texture: public Resource{
	public:
		Texture();
		unsigned textureID = 0;
		TextureTarget target = TextureTarget::Texture2D;
		glm::ivec2 size = glm::ivec2(0);

		void setSize(unsigned width, unsigned height);
		void setTexture(unsigned char* data, unsigned inFormat = GL_RGBA, unsigned outFormat = GL_RGBA, unsigned filter = GL_LINEAR);
		void setSkyBox(std::array<unsigned char*, 6> data, unsigned inFormat = GL_RGBA, unsigned outFormat = GL_RGBA, unsigned filter = GL_LINEAR);
	};
}
