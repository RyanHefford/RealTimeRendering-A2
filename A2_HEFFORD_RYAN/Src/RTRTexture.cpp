#include "RTRTexture.h"
#ifndef STB_IMAGE_IMPLEMENTATION
	#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION

void RTRTexture::LoadTexture(const char* path)
{
	stbi_set_flip_vertically_on_load(true);

	glGenTextures(1, &m_Id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Id);

	//set wrapping options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//load in texture
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 4);

	if (data) {

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture at: " << path << std::endl;
	}
	stbi_image_free(data);
	Unbind();
}

void RTRTexture::Bind(int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, m_Id);
}

void RTRTexture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

RTRTexture::~RTRTexture() {
	glDeleteTextures(1, &m_Id);
}
