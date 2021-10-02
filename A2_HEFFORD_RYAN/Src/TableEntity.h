#pragma once
#include "RTRShader.h"
#include "RTRObject.h"

class TableEntity {
public:
	float xPos;
	float yPos;
	RTRObject* m_Geometry = {nullptr};

	TableEntity(float _xPos, float _yPos);

	virtual void RenderGeometry(RTRShader* shader);

	void End();
};

//-------------------------------------------------------

class Edge : public TableEntity {
public:
	Edge(float width, float height, float _xPos, float _yPos, unsigned int texId, glm::vec3 rotation = glm::vec3(0), float rotationAngle = 0);
	

private:

};

//-------------------------------------------------------

class Backboard : public TableEntity {
public:
	Backboard(float width, float height, unsigned int texId, float _xPos = 0, float _yPos = 0);
	

private:

};

//-------------------------------------------------------

class PinBall : public TableEntity {
public:
	PinBall(float radius, float _xPos, float _yPos);
	
};

//-------------------------------------------------------

class Table : public TableEntity {
public:
	Table(float width, float height, unsigned int texId, float _xPos = 0, float _yPos = 0);
	virtual void RenderGeometry(RTRShader* shader);

private:
	float borderThickness = 0.05f;
	unsigned int numParts = 0;
	TableEntity* tableParts[30];
};