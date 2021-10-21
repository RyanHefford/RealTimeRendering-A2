#pragma once
#include "RTRObject.h"

struct RTRAABB
{
	glm::vec3 min;
	glm::vec3 max;
};

struct Collision
{
	bool isCollision;
	glm::vec3 direction;
	glm::vec3 difference;
};

class TableEntity {
public:
	glm::vec3 position;

	float gravity = 100.0f;
	RTRObject* m_Geometry = {nullptr};

	TableEntity(float _xPos, float _yPos, float _zPos);

	virtual void RenderGeometry(RTRShader* shader);
	RTRObject* GetGeometry() { return m_Geometry; }

	void End();
};

//-------------------------------------------------------

class PinBall : public TableEntity {
private:

public:
	glm::vec3 v = { 0,0,0 }, r = { 0,0,0 };
	bool hasShot = false;
	float radius;
	bool updated = true;

	PinBall(float _radius, float _xPos, float _yPos, float _zPos, RTRMaterial_t* material);
	void AddForce(glm::vec3 force);
	void UpdateState(unsigned int td_milli);
};

//-------------------------------------------------------

class Edge : public TableEntity {
public:
	Edge(float width, float height, float depth, float _xPos, float _yPos, float _zPos, RTRMaterial_t* material, glm::vec3 rotation = glm::vec3(0), float rotationAngle = 0);
	Collision CheckCollision(unsigned int td_milli, PinBall* ball);
	glm::vec3 VectorDirection(glm::vec3 target);
private:
	RTRAABB boundingBox;
};

//-------------------------------------------------------

class CornerPiece : public TableEntity {
public:
	CornerPiece(float scale, float _xPos, float _yPos, float _zPos, RTRMaterial_t* material);


private:

};

//-------------------------------------------------------

class Backboard : public TableEntity {
public:
	Backboard(float width, float height, RTRMaterial_t* material, float _xPos = 0, float _yPos = 0, float _zPos = 0);
	

private:

};

//-------------------------------------------------------

class Triangle : public TableEntity {
public:
	Triangle(float width, float height, float depth, float _xPos, float _yPos, float _zPos, RTRMaterial_t* material, glm::vec3 rotation = glm::vec3(0), float rotationAngle = 0);
	Collision CheckCollision(unsigned int td_milli, PinBall* ball);

	glm::vec3 VectorDirection(glm::vec3 target);

private:
	RTRAABB boundingBox;
};

//-------------------------------------------------------


class Plunger : public TableEntity {
public:
	Plunger(float radius, float height, unsigned int sectors, RTRMaterial_t* material, float _xPos = 0, float _yPos = 0, float _zPos = 0, glm::vec3 rotation = glm::vec3(0), float rotationAngle = 0);
	void UpdatePosition(float percentagePulled);

	void ResetPosition();

private:
	glm::mat4 startPosition;
	glm::vec3 maxYTranslate;
};