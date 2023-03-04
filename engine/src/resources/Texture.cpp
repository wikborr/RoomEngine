#include "resources/Texture.h"

namespace RoomEng{
	Texture::Texture(){
		glGenTextures(1, &this->textureID);
	}

	void Texture::setSize(unsigned width, unsigned height){
		this->size = glm::ivec2(width, height);
	}

	void Texture::setTexture(unsigned char* data, unsigned inFormat, unsigned outFormat, unsigned filter){
		glBindTexture(GL_TEXTURE_2D, this->textureID);
		this->target = TextureTarget::Texture2D;
	
		//texture properties
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

		//set the texture in the OpenGL context
		glTexImage2D(GL_TEXTURE_2D, 0, outFormat, this->size.x, this->size.y, 0, inFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::setSkyBox(std::array<unsigned char*, 6> data, unsigned inFormat, unsigned outFormat, unsigned filter){
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);
		this->target = TextureTarget::Cubemap;

		//texture properties
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filter);

		//set the texture in the OpenGL context
		for(int i = 0; i < data.size(); i++){
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, outFormat, this->size.x, this->size.y, 0, inFormat, GL_UNSIGNED_BYTE, data.at(i)
			);
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}