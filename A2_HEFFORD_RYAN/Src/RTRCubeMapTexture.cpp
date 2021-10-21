#include "RTRCubeMapTexture.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION
void RTRCubeMapTexture::LoadTexture(const char** path)
{
	stbi_set_flip_vertically_on_load(false);

	glGenTextures(1, &m_Id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Id);

	//set wrapping options
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	for (int i = 0; i < 6; i++) {
		//load in texture
		unsigned char* data = stbi_load(path[i], &width, &height, &nrChannels, 4);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load texture at: " << path << std::endl;
		}
		stbi_image_free(data);
	}
	Unbind();
}

void RTRCubeMapTexture::Bind(int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Id);
}

void RTRCubeMapTexture::Unbind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

RTRCubeMapTexture::~RTRCubeMapTexture() {
	glDeleteTextures(1, &m_Id);
}
