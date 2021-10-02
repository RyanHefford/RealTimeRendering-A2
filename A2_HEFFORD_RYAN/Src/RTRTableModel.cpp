#include "RTRTableModel.h"

RTRTableModel::RTRTableModel(float _width, float _height)
{
	width = _width;
	height = _height;

	//create textures
	metalTexture = new RTRTexture(".\\Src\\MetalTexture.jpg");
	backboardTexture = new RTRTexture(".\\Src\\PaintedWoodTexture.png");


	float halfWidth = width / 2;
	float halfHeight = height / 2;
	borderThickness = glm::sqrt(width + height) / 30;

	//Add each border edge to list
	tableParts[numParts] = new Edge(halfWidth, borderThickness, 0, halfHeight, metalTexture->m_Id);	//top border
	numParts++;
	tableParts[numParts] = new Edge(borderThickness, halfHeight, halfWidth, 0, metalTexture->m_Id);	//right border
	numParts++;
	tableParts[numParts] = new Edge(halfWidth, borderThickness, 0, -halfHeight, metalTexture->m_Id);	//bottom border
	numParts++;
	tableParts[numParts] = new Edge(borderThickness, halfHeight, -halfWidth, 0, metalTexture->m_Id);	//left border
	numParts++;

	//calculate inlane
	float xOffset = width / 4;
	float heightDecrease = halfHeight / 4;
	float yOffset = -halfHeight + 3 * heightDecrease;

	tableParts[numParts] = new Edge(borderThickness / 2, halfHeight - heightDecrease, halfWidth - xOffset, yOffset, metalTexture->m_Id);
	numParts++;

	//add inlane cornerpiece
	float cornerPieceLength = halfHeight / 8;
	float angleInRadians = glm::radians<float>(-45.0f);
	tableParts[numParts] = new Edge(heightDecrease, borderThickness / 2, halfWidth - 3 * borderThickness, halfHeight - 3 * borderThickness, metalTexture->m_Id, glm::vec3(0, 0, 1), angleInRadians);
	numParts++;

	//add backboard
	tableParts[numParts] = new Backboard(halfWidth, halfHeight, backboardTexture->m_Id);
	numParts++;
}

RTRTableModel::~RTRTableModel()
{
}

void RTRTableModel::End()
{
	
}

void RTRTableModel::RenderTable(RTRShader* shader)
{
	for (int i = 0; i < numParts; i++)
	{
		tableParts[i]->RenderGeometry(shader);
	}
}
