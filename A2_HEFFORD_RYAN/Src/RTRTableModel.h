#pragma once
#include "TableEntity.h"


class RTRTableModel {
public:
	RTRTableModel(float _width, float _height);
	~RTRTableModel();

	void End();

	void RenderTable(RTRShader* shader);

private:
	float width;
	float height;

	float borderThickness = 0.05f;
	unsigned int numParts = 0;
	TableEntity* tableParts[30];
	RTRTexture* metalTexture;
	RTRTexture* backboardTexture;

};