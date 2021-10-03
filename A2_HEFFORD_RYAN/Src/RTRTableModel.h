#pragma once
#include "TableEntity.h"
#include "RTRSkyBox.h"
#include "RTRCubeMapTexture.h"

class RTRTableModel {
public:
	RTRTableModel(float _width, float _height);
	~RTRTableModel();

	void End();

	void RenderTable(RTRShader* shader);
	void RenderSkyBox(RTRShader* shader);

private:
	float width;
	float height;

	float borderThickness = 0.05f;
	unsigned int numParts = 0;
	TableEntity* tableParts[30];
	RTRTexture* metalTexture;
	RTRTexture* backboardTexture;
	RTRCubeMapTexture* skyBoxTexture;
	RTRSkyBox* skyBox;
};