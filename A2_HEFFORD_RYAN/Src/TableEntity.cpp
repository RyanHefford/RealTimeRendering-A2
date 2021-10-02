#include "TableEntity.h"

TableEntity::TableEntity(float _xPos, float _yPos)
{
	xPos = _xPos;
	yPos = _yPos;
}

void TableEntity::RenderGeometry(RTRShader* shader)
{
	m_Geometry->Render(shader);
}

void TableEntity::End()
{
	//todo
}

//-------------------------------------------------------------------------


Edge::Edge(float width, float height, float _xPos, float _yPos, unsigned int texId, glm::vec3 rotation, float rotationAngle) : TableEntity(_xPos, _yPos)
{
	m_Geometry = new RTRRectangle(width, height, 0.1f, glm::vec3(xPos, yPos, 0.1f), texId, rotation, rotationAngle);
	m_Geometry->Init();
}


//-------------------------------------------------------------------------

Backboard::Backboard(float width, float height, unsigned int texId, float _xPos, float _yPos) : TableEntity(_xPos, _yPos)
{
	m_Geometry = new RTRRectangle(width, height, 0.01f, glm::vec3(xPos, yPos, 0.0f), texId);
	m_Geometry->Init();
}


//-------------------------------------------------------------------------

PinBall::PinBall(float radius, float _xPos, float _yPos) : TableEntity(_xPos, _yPos)
{
}


//-------------------------------------------------------------------------


Table::Table(float width, float height, unsigned int texId, float _xPos, float _yPos) : TableEntity(_xPos, _yPos)
{
	float halfWidth = width / 2;
	float halfHeight = height / 2;
	borderThickness = glm::sqrt(width + height) / 30;

	//Add each border edge to list
	tableParts[numParts] = new Edge(halfWidth, borderThickness, 0, halfHeight, texId);	//top border
	numParts++;
	tableParts[numParts] = new Edge(borderThickness, halfHeight, halfWidth, 0, texId);	//right border
	numParts++;
	tableParts[numParts] = new Edge(halfWidth, borderThickness, 0, -halfHeight, texId);	//bottom border
	numParts++;
	tableParts[numParts] = new Edge(borderThickness, halfHeight, -halfWidth, 0, texId);	//left border
	numParts++;

	//calculate inlane
	float xOffset = width/4;
	float heightDecrease = halfHeight / 4;
	float yOffset = -halfHeight + 3*heightDecrease;

	tableParts[numParts] = new Edge(borderThickness / 2, halfHeight - heightDecrease, halfWidth - xOffset, yOffset, texId);
	numParts++;

	//add inlane cornerpiece
	float cornerPieceLength = halfHeight / 8;
	float angleInRadians = glm::radians<float>(-45.0f);
	tableParts[numParts] = new Edge(heightDecrease, borderThickness/2, halfWidth - 3*borderThickness, halfHeight - 3 * borderThickness, texId, glm::vec3(0,0,1), angleInRadians);
	numParts++;

	//add backboard
	tableParts[numParts] = new Backboard(halfWidth, halfHeight, texId);
	numParts++;
}

void Table::RenderGeometry(RTRShader* shader)
{
	for (unsigned int i = 0; i < numParts; i++) {
		tableParts[i]->RenderGeometry(shader);
	}
}

