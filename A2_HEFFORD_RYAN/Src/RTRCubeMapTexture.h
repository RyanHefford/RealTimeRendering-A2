#pragma once

#include"stb/stb_image.h"
#include <glad/glad.h>
#include <iostream>

class RTRCubeMapTexture {
public:
	RTRCubeMapTexture(const char** path) { LoadTexture(path); }
	~RTRCubeMapTexture();
	void LoadTexture(const char** path);

	void Bind(int index);
	void Unbind();

	unsigned int m_Id;
	int width, height, nrChannels;
};