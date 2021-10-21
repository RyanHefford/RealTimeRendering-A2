#include "RTRTableModel.h"

RTRTableModel::RTRTableModel(float _width, float _height)
{
	width = _width;
	height = _height;

	//create textures
	metalTexture = new RTRTexture(".\\Resources\\Textures\\UsedMetal.png");
	metalEdgeTexture = new RTRTexture(".\\Resources\\Textures\\UsedMetalEdge.png");
	smoothWoodTexture = new RTRTexture(".\\Resources\\Textures\\DarkWood.png");
	backboardTexture = new RTRTexture(".\\Resources\\Textures\\PaintedWoodTexture.png");
	glassTexture = new RTRTexture(".\\Resources\\Textures\\Glass.jpg");

	//create Materials
	metalMaterial = new RTRMaterial_t{ RTRMaterialType::ReflectiveMaterial, {0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 }, 128.0, metalTexture, metalTexture, metalTexture};
	woodMetalBorder = new RTRMaterial_t{ RTRMaterialType::ReflectiveMaterial, {0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 }, 128.0, smoothWoodTexture, smoothWoodTexture, metalEdgeTexture };
	backboardMaterial = new RTRMaterial_t{ RTRMaterialType::TexturedMaterial, {0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 }, 128.0, backboardTexture, backboardTexture, backboardTexture };
	glassMaterial = new RTRMaterial_t{ RTRMaterialType::GlassMaterial, {0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, 120.0, glassTexture, glassTexture, glassTexture, 1.52 };
	smoothWoodMaterial = new RTRMaterial_t{ RTRMaterialType::TexturedMaterial, {0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 }, 128.0, smoothWoodTexture, smoothWoodTexture, smoothWoodTexture };

	float halfWidth = width / 2;
	float halfHeight = height / 2;
	borderThickness = glm::sqrt(width + height) / 30;
	float horizontalSpacing = halfWidth + borderThickness;
	float verticalSpacing = halfHeight + borderThickness;

	//Add each border edge to list
	tableParts[numParts] = new Edge(halfWidth, borderThickness, borderThickness, 0, verticalSpacing, 0.1, smoothWoodMaterial);	//top border
	numParts++;
	tableParts[numParts] = new Edge(borderThickness, halfHeight, borderThickness, horizontalSpacing, 0, 0.1, smoothWoodMaterial);	//right border
	numParts++;
	tableParts[numParts] = new Edge(halfWidth, borderThickness, borderThickness, 0, -verticalSpacing, 0.1, smoothWoodMaterial);	//bottom border
	numParts++;
	tableParts[numParts] = new Edge(borderThickness, halfHeight, borderThickness, -horizontalSpacing, 0, 0.1, smoothWoodMaterial);	//left border
	numParts++;

	//Add corner pieces
	tableParts[numParts] = new CornerPiece(borderThickness, -horizontalSpacing, verticalSpacing, 0.1f, woodMetalBorder);	//top left
	numParts++;
	tableParts[numParts] = new CornerPiece(borderThickness, horizontalSpacing, verticalSpacing, 0.1f, woodMetalBorder);	//top right
	numParts++;
	tableParts[numParts] = new CornerPiece(borderThickness, -horizontalSpacing, -verticalSpacing, 0.1f, woodMetalBorder);	//bottom left
	numParts++;
	tableParts[numParts] = new CornerPiece(borderThickness, horizontalSpacing, -verticalSpacing, 0.1f, woodMetalBorder);	//bottom right
	numParts++;


	//calculate inlane
	float xOffset = width / 4;
	float heightDecrease = halfHeight / 4;
	float yOffset = -halfHeight + 3 * heightDecrease;
	float inlaneSize = 3 * borderThickness;

	tableParts[numParts] = new Edge(borderThickness, halfHeight - heightDecrease, borderThickness, horizontalSpacing - xOffset, yOffset, 0.1, smoothWoodMaterial);
	numParts++;

	//add inlane cornerpiece
	float cornerPieceLength = halfHeight / 8;
	float angleInRadians = glm::radians<float>(-90.0f);
	//tableParts[numParts] = new Triangle(heightDecrease/2, heightDecrease/2, borderThickness, horizontalSpacing - inlaneSize, verticalSpacing - inlaneSize, 0.1, woodMetalBorder, glm::vec3(0, 0, 1), angleInRadians);
	//numParts++;


	//add backboard
	tableParts[numParts] = new Backboard(halfWidth, halfHeight, backboardMaterial);
	numParts++;

	//add glass
	//tableParts[numParts] = new Backboard(halfWidth, halfHeight, glassMaterial, 0, 0, 0.2f);
	//numParts++;

	//add plunger and assign its index
	plunger = new Plunger(borderThickness/2, 8, 64, metalMaterial, horizontalSpacing - inlaneSize, -verticalSpacing , 0.1);
	tableParts[numParts] = plunger;
	numParts++;

	ballSpawnPos = { horizontalSpacing - inlaneSize , -verticalSpacing + 0.3 , 0.1};

	//add skybox
	// order of files matches GL_TEXTURE_CUBE_MAP_XXX id order
	const char* skyBoxPath[] = {
		".\\Resources\\Textures\\Skybox\\px.png", 
		".\\Resources\\Textures\\Skybox\\nx.png",
		".\\Resources\\Textures\\Skybox\\py.png",
		".\\Resources\\Textures\\Skybox\\ny.png",
		".\\Resources\\Textures\\Skybox\\pz.png",
		".\\Resources\\Textures\\Skybox\\nz.png",
	};
	skyBoxTexture = new RTRCubeMapTexture(skyBoxPath);
	skyBoxMaterial = new RTRMaterial_t{ RTRMaterialType::SkyboxMaterial, {0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 }, 0.0, nullptr, nullptr, nullptr, 0, skyBoxTexture  };
	skyBox = new RTRSkyBox(skyBoxMaterial);
}

RTRTableModel::~RTRTableModel()
{
}

void RTRTableModel::End()
{
	
}

void RTRTableModel::RenderTable(RTRShader* shader)
{
	shader->SetInt("u_SkyBox", 0);
	skyBoxTexture->Bind(0);
	for (int i = 0; i < numParts; i++)
	{
		tableParts[i]->GetGeometry()->m_Material->DiffuseMap->Bind(tableParts[i]->GetGeometry()->m_Material->DiffuseMap->m_Id);
		tableParts[i]->GetGeometry()->m_Material->SpecularMap->Bind(tableParts[i]->GetGeometry()->m_Material->SpecularMap->m_Id);
		tableParts[i]->GetGeometry()->m_Material->ReflectionMap->Bind(tableParts[i]->GetGeometry()->m_Material->ReflectionMap->m_Id);

		tableParts[i]->RenderGeometry(shader);
	}

	for (int i = 0; i < numPinballs; i++)
	{
		if (pinballCollection[i] != nullptr) {
			pinballCollection[i]->GetGeometry()->m_Material->DiffuseMap->Bind(pinballCollection[i]->GetGeometry()->m_Material->DiffuseMap->m_Id);
			pinballCollection[i]->GetGeometry()->m_Material->SpecularMap->Bind(pinballCollection[i]->GetGeometry()->m_Material->SpecularMap->m_Id);
			pinballCollection[i]->GetGeometry()->m_Material->ReflectionMap->Bind(pinballCollection[i]->GetGeometry()->m_Material->ReflectionMap->m_Id);

			pinballCollection[i]->RenderGeometry(shader);
		}
	}
}

void RTRTableModel::RenderSkyBox(RTRShader* shader)
{
	skyBox->Render(shader);
}

void RTRTableModel::UpdatePlunger(bool increasePower, unsigned int td_milli) {
	if (increasePower) {
		//increase current plunger power
		float multipier = maxPlungerPower / 2;
		
		currentPlungerPower = maxPlungerPower <= currentPlungerPower ? maxPlungerPower : currentPlungerPower + (multipier * (float)td_milli / 1000);
		plunger->UpdatePosition(currentPlungerPower /maxPlungerPower);

		if (currentPinball == nullptr) {
			currentPinball = new PinBall(0.05,ballSpawnPos.x, ballSpawnPos.y, 0.1, metalMaterial);
			pinballCollection[numPinballs] = currentPinball;
			numPinballs++;
		}
	}
	else if (currentPlungerPower != 0) {
		//fire
		currentPinball->hasShot = true;
		currentPinball->AddForce(glm::vec3(20.0f,currentPlungerPower,0));
		//empty variables for next ball
		currentPinball = nullptr;
		currentPlungerPower = 0;

		plunger->ResetPosition();
	}
}

void RTRTableModel::UpdateState(unsigned int td_milli)
{

	for (int i = 0; i < numPinballs; i++) {
		if (pinballCollection[i]->hasShot) {
			if (pinballCollection[i]->updated) {
				for (int j = 0; j < numParts; j++) {
					Edge* tempEdge = dynamic_cast<Edge*>(tableParts[j]);
					Triangle* tempTriangle = dynamic_cast<Triangle*>(tableParts[j]);

					if (tempEdge) {

						Collision collision;
						collision = tempEdge->CheckCollision(td_milli, pinballCollection[i]);

						ApplyStaticCollision(collision, pinballCollection[i]);
					}
					else if (tempTriangle) {
						Collision collision;
						collision = tempTriangle->CheckCollision(td_milli, pinballCollection[i]);

						ApplyStaticCollision(collision, pinballCollection[i]);
					}
				}
			}
			

			for (int j = 0; j < numPinballs; j++) {
				if (j != i && pinballCollection[i]->updated) {
					CheckBallCollision(pinballCollection[i], pinballCollection[j]);
				}
			}

			pinballCollection[i]->UpdateState(td_milli); 
			/*if (pinballCollection[i]->position.y < -10.0f) {
				delete(pinballCollection[i]);
				pinballCollection[i] = nullptr;

				numPinballs--;
			}*/
		}
	}
}

void RTRTableModel::ApplyStaticCollision(Collision collision, PinBall* ball)
{
	if (collision.isCollision) {
		float friction = 0.95;
		float restrution = 0.5;

		glm::vec3 u = (glm::dot(ball->v, collision.direction) / glm::dot(collision.direction, collision.direction)) * collision.direction;
		glm::vec3 w = ball->v - u;

		ball->v = friction * w - restrution * u;

		//horizontal
		if (collision.direction.x != 0) {
			float penetration = ball->radius - glm::abs(collision.difference.x);

			//left
			if (collision.direction.x == -1) {
				ball->position.x += penetration;
			}
			//right
			else {
				ball->position.x -= penetration;
			}

		}
		//vertical
		if(collision.direction.y != 0) {
			float penetration = ball->radius - glm::abs(collision.difference.y);

			//down
			if (collision.direction.y == -1) {
				ball->position.y -= penetration;
			}
			//up
			else {
				ball->position.y += penetration;
			}
		}
	}
}

void RTRTableModel::CheckBallCollision(PinBall* ball1, PinBall* ball2)
{

	//check if there is a collision
	float distance = glm::sqrt(
		(ball1->position.x - ball2->position.x) * (ball1->position.x - ball2->position.x) +
		(ball1->position.y - ball2->position.y) * (ball1->position.y - ball2->position.y) +
		(ball1->position.z - ball2->position.z) * (ball1->position.z - ball2->position.z)
	);

	if (distance < (ball1->radius + ball2->radius)) {
		ball1->updated = false;
		ball2->updated = false;

		//handle vellocity changes
		glm::vec3 vecX = glm::normalize(ball1->position - ball2->position);

		//calculate x direction velocity vector and y vector
		glm::vec3 vecV1 = ball1->v;
		float x1 = glm::dot(vecX, vecV1);
		glm::vec3 vecV1X = vecX * x1;
		glm::vec3 vecV1Y = vecV1 - vecV1X;
		float m1 = 0.1f; //mass

		//same for other ball
		vecX *= -1;
		glm::vec3 vecV2 = ball2->v;
		float x2 = glm::dot(vecX, vecV2);
		glm::vec3 vecV2X = vecX * x2;
		glm::vec3 vecV2Y = vecV2 - vecV2X;
		float m2 = 0.1f; //mass

		ball1->v = vecV1X*(m1 - m2) / (m1 + m2) + vecV2X*(2 * m2) / (m1 + m2) + vecV1Y;
		ball2->v = vecV1X * (2 * m1) / (m1 + m2) + vecV2X * (m2 - m1) / (m1 + m2) + vecV2Y;
	}

	//move balls away from each other
	float overlap = 0.5f * (distance - ball1->radius - ball2->radius);

	ball1->position.x -= overlap * (ball1->position.x - ball2->position.x) / distance;
	ball1->position.y -= overlap * (ball1->position.y - ball2->position.y) / distance;

	ball2->position.x += overlap * (ball2->position.x - ball1->position.x) / distance;
	ball2->position.y += overlap * (ball2->position.y - ball1->position.y) / distance;

}
