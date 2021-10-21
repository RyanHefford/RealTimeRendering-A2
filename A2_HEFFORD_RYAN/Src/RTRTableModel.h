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
	void UpdatePlunger(bool increasePower, unsigned int td_milli);
	void UpdateState(unsigned int td_milli);
	void ApplyStaticCollision(Collision collision, PinBall* ball);
	void CheckBallCollision(PinBall* ball1, PinBall* ball2);
	int GetNumPinballs() { return numPinballs; }
private:
	float width;
	float height;

	float borderThickness = 0.05f;
	unsigned int numParts = 0;
	unsigned int numPinballs = 0;
	TableEntity* tableParts[30];
	PinBall* pinballCollection[50];

	//Plunger
	float maxPlungerPower = 200.0f;
	float currentPlungerPower = 0;
	unsigned int plungerObjectIndex = 0;
	PinBall* currentPinball;
	Plunger* plunger;
	glm::vec3 ballSpawnPos;

	//Textures
	RTRTexture* metalTexture;
	RTRTexture* metalEdgeTexture;
	RTRTexture* backboardTexture;
	RTRTexture* glassTexture;
	RTRTexture* smoothWoodTexture;

	//Materials
	RTRMaterial_t* metalMaterial = new RTRMaterial_t;
	RTRMaterial_t* backboardMaterial = new RTRMaterial_t;
	RTRMaterial_t* smoothWoodMaterial = new RTRMaterial_t;
	RTRMaterial_t* woodMetalBorder = new RTRMaterial_t;
	RTRMaterial_t* skyBoxMaterial = new RTRMaterial_t;;
	RTRMaterial_t* glassMaterial = new RTRMaterial_t;
	
	//SkyBox
	RTRCubeMapTexture* skyBoxTexture;
	RTRSkyBox* skyBox;
};