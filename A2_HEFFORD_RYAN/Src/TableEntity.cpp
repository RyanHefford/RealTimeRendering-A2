#include "TableEntity.h"

TableEntity::TableEntity(float _xPos, float _yPos, float _zPos)
{
	position = glm::vec3(_xPos, _yPos, _zPos);
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


Edge::Edge(float width, float height, float depth, float _xPos, float _yPos, float _zPos, RTRMaterial_t* material, glm::vec3 rotation, float rotationAngle) : TableEntity(_xPos, _yPos, _zPos)
{
	m_Geometry = new RTRRectangle(width, height, depth, glm::vec3(_xPos, _yPos, _zPos), material, rotation, rotationAngle);
	m_Geometry->Init();

	boundingBox.min = glm::vec3(_xPos - width, _yPos - height, _zPos - depth);
	boundingBox.max = glm::vec3(_xPos + width, _yPos + height, _zPos + depth);
}

Collision Edge::CheckCollision(unsigned int td_milli, PinBall* ball) {

	glm::mat4 tempMatrix = glm::translate(ball->m_Geometry->m_ModelMatrix, ball->v * ((float)td_milli / 1000.0f));
	glm::vec3 nextPosition = tempMatrix[3];

	position = m_Geometry->m_ModelMatrix[3];

	//finding closest point to sphere's center through clamping
	float x = glm::max(boundingBox.min.x, glm::min(nextPosition.x, boundingBox.max.x));
	float y = glm::max(boundingBox.min.y, glm::min(nextPosition.y, boundingBox.max.y));
	float z = glm::max(boundingBox.min.z, glm::min(nextPosition.z, boundingBox.max.z));

	float distance = glm::sqrt(
		(x - nextPosition.x) * (x - nextPosition.x) +
		(y - nextPosition.y) * (y - nextPosition.y) +
		(z - nextPosition.z) * (z - nextPosition.z)
	);

	glm::vec3 difference = glm::vec3(x,y,z) - nextPosition;
	if (difference == glm::vec3(0)) { difference = glm::vec3(x, y, z) - ball->position; }

	Collision result = { false, glm::vec3(0), glm::vec3(0) };

	if (distance < ball->radius) { 
		ball->updated = false;
		result = { true, VectorDirection(difference), difference };
	}

	return result;
}


glm::vec3 Edge::VectorDirection(glm::vec3 target)
{
	glm::vec3 compass[] = {
	glm::vec3(0.0f, 1.0f, 0.0f), // up
	glm::vec3(1.0f, 0.0f, 0.0f), // right
	glm::vec3(0.0f, -1.0f, 0.0f), // down
	glm::vec3(-1.0f, 0.0f, 0.0f) // left
	};
	float max = 0.0f;
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(target), compass[i]);

		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}

	return compass[best_match];
}

//-------------------------------------------------------------------------

Backboard::Backboard(float width, float height, RTRMaterial_t* material, float _xPos, float _yPos, float _zPos) : TableEntity(_xPos, _yPos, _zPos)
{
	m_Geometry = new RTRRectangle(width, height, 0.05f, glm::vec3(_xPos, _yPos, _zPos), material);
	m_Geometry->Init();
}


//-------------------------------------------------------------------------

PinBall::PinBall(float _radius, float _xPos, float _yPos, float _zPos, RTRMaterial_t* material) : TableEntity(_xPos, _yPos, _zPos)
{
	m_Geometry = new RTRSphere(_radius, 160, 160, glm::vec3(_xPos, _yPos, _zPos), material);
	m_Geometry->Init();

	radius = _radius;
}

void PinBall::AddForce(glm::vec3 force)
{
	v += force;
}

void PinBall::UpdateState(unsigned int td_milli)
{
	m_Geometry->m_ModelMatrix = glm::translate(m_Geometry->m_ModelMatrix, v*((float)td_milli/1000.0f));

	position = m_Geometry->m_ModelMatrix[3];

	v.y -= gravity * (float)td_milli/1000.0f;

	updated = true;

	glBegin(GL_LINES);
	glColor3d(1, 0, 0);
	glVertex3f(position.x, position.y, position.z);
	glVertex3f(v.x, v.y, v.z);
	glEnd();
}


//-------------------------------------------------------------------------


Plunger::Plunger(float radius, float height, unsigned int sectors, RTRMaterial_t* material, float _xPos, float _yPos, float _zPos, glm::vec3 rotation, float rotationAngle) : TableEntity(_xPos, _yPos, _zPos)
{
	m_Geometry = new RTRCylinder(radius, height, sectors, glm::vec3(_xPos, _yPos, _zPos), material);
	m_Geometry->Init();

	startPosition = m_Geometry->m_ModelMatrix;
	maxYTranslate = glm::vec3(0,-2,0);
}

void Plunger::UpdatePosition(float percentagePulled)
{
	if (percentagePulled < 1.0f) {
		m_Geometry->m_ModelMatrix = glm::translate(startPosition, maxYTranslate * percentagePulled);
	}
}

void Plunger::ResetPosition() {
	m_Geometry->m_ModelMatrix = startPosition;
}

//-------------------------------------------------------------------------

Triangle::Triangle(float width, float height, float depth, float _xPos, float _yPos, float _zPos, RTRMaterial_t* material, glm::vec3 rotation, float rotationAngle) : TableEntity(_xPos, _yPos, _zPos)
{
	m_Geometry = new RTRTriangularPrism(width, height, depth, glm::vec3(_xPos, _yPos, _zPos), material, rotation, rotationAngle);
	m_Geometry->Init();

	boundingBox.min = glm::vec3(_xPos - width, _yPos - height, _zPos - depth);
	boundingBox.max = glm::vec3(_xPos + width, _yPos + height, _zPos + depth);
}

Collision Triangle::CheckCollision(unsigned int td_milli, PinBall* ball) {

	glm::mat4 tempMatrix = glm::translate(ball->m_Geometry->m_ModelMatrix, ball->v * ((float)td_milli / 1000.0f));
	glm::vec3 nextPosition = tempMatrix[3];

	position = m_Geometry->m_ModelMatrix[3];

	//finding closest point to sphere's center through clamping
	float x = glm::max(boundingBox.min.x, glm::min(nextPosition.x, boundingBox.max.x));
	float y = glm::max(boundingBox.min.y, glm::min(nextPosition.y, boundingBox.max.y));
	float z = glm::max(boundingBox.min.z, glm::min(nextPosition.z, boundingBox.max.z));

	float distance = glm::sqrt(
		(x - nextPosition.x) * (x - nextPosition.x) +
		(y - nextPosition.y) * (y - nextPosition.y) +
		(z - nextPosition.z) * (z - nextPosition.z)
	);

	glm::vec3 difference = glm::vec3(x, y, z) - nextPosition;
	if (difference == glm::vec3(0)) { difference = glm::vec3(x, y, z) - ball->position; }

	Collision result = { false, glm::vec3(0), glm::vec3(0) };

	if (distance < ball->radius) {

		result = { true, VectorDirection(difference), difference };
	}

	return result;
}



glm::vec3 Triangle::VectorDirection(glm::vec3 target)
{
	glm::vec3 compass[] = {
	glm::vec3(0.0f, 1.0f, 0.0f), // up
	glm::vec3(1.0f, 0.0f, 0.0f), // right
	glm::vec3(0.0f, -1.0f, 0.0f), // down
	glm::vec3(-1.0f, 0.0f, 0.0f) // left
	};
	float max = 0.0f;
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(target), compass[i]);

		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	std::cout << "new target" << std::endl;
	std::cout << target.x << std::endl;
	std::cout << target.y << std::endl;
	std::cout << target.z << std::endl;
	std::cout << best_match << std::endl;

	glm::vec3 result = compass[best_match];
	if (best_match == 0 || best_match == 1) {
		result = glm::vec3(-1.0f, -1.0f, 0.0f);
	}
	glm::normalize(result);
	return compass[best_match];
}

//-------------------------------------------------------------------------

CornerPiece::CornerPiece(float scale, float _xPos, float _yPos, float _zPos, RTRMaterial_t* material) : TableEntity(_xPos, _yPos, _zPos)
{
	m_Geometry = new RTRCube(scale, glm::vec3(_xPos, _yPos, _zPos), material);
	m_Geometry->Init();
}
