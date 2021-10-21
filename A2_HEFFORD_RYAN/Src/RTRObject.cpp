//-----------------------------------------------------------------------------
// RTRObject.cpp
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#include <iostream>
#include "RTRObject.h"

// README / TODO - This is just a simple start.
// * For your assignment, think carefully about how Shaders, Materials and Objects will hang together.
// * Have a hierarchy of differnet object types (static object, moving object, hierarchical object, object with force, etc...
// * Don't worry too much about optimising the buffer handling by, for example, merging all objects into a single 
// vertex buffer. Or sorting object based on shader or material to minimize switches. Spend your time on more imprtant 
// optimisation techniques that will add marks - for example your uniform grid implementation. 
// * For this assignment you can hardcode the vertex buffers for the differnet shapes (see cube) and then 
// scale, place and animate them with transformation matrices.

RTRObject::RTRObject(RTRMaterial_t* material, glm::vec3 position)
{
    m_ModelMatrix = glm::translate(m_ModelMatrix, position);
    m_Material = material;
}

//-----------------------------------------------------------------------------
void RTRObject::Init()
{
    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);

    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_NumVertices * sizeof(RTRPoint_t), m_VertexPoints, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RTRPoint_t), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_TexCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_TexCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_NumTexCoords * sizeof(RTRTexCoord_t), m_TexCoords, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(RTRTexCoord_t), (void*)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &m_FaceElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_FaceElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NumFaces * sizeof(RTRFace_t), m_Faces, GL_STATIC_DRAW);
}

void RTRObject::Render(RTRShader *shader)
{
    shader->SetMat4("u_ModelMatrix", m_ModelMatrix);
    shader->SetMaterial("u_ObjectMaterial", *m_Material);
    glBindVertexArray(m_VertexArray);
    glDrawElements(GL_TRIANGLES, m_NumFaces * 3, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, m_NumVertices);
    glBindVertexArray(0);
}

void RTRObject::End()
{
    glDeleteVertexArrays(1, &m_VertexArray); m_VertexArray = 0;
    glDeleteBuffers(1, &m_VertexBuffer); m_VertexBuffer = 0;
    glDeleteBuffers(1, &m_FaceElementBuffer); m_FaceElementBuffer = 0;
    if (m_VertexPoints != nullptr) { delete m_VertexPoints; m_VertexPoints = nullptr; }
    if (m_Faces != nullptr) { delete m_Faces; m_Faces = nullptr; }
}


RTRRectangle::RTRRectangle(float _width, float _height, float _depth, glm::vec3 position, RTRMaterial_t* material, glm::vec3 rotation, float rotationAngleInRadians) : RTRObject(material, position)
{
    if (rotationAngleInRadians != 0) { m_ModelMatrix = glm::rotate(m_ModelMatrix, rotationAngleInRadians, rotation); }
    m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(_width, _height, _depth));
}

void RTRRectangle::Init()
{
    m_NumVertices = 36;
    m_NumTexCoords = 36;
    m_NumFaces = 12;

    
    m_VertexPoints = new RTRPoint_t[]{
        {  1, -1,  1 },{  1, -1, -1 },{  1,  1, -1 },
        {  1, -1,  1 },{  1,  1, -1 },{  1,  1,  1 },
        
        { -1, -1, -1 },{ -1, -1,  1 },{ -1,  1,  1 },
        { -1, -1, -1 },{ -1,  1,  1 },{ -1,  1, -1 },

        { -1,  1,  1 },{  1,  1,  1 },{  1,  1, -1 },
        { -1,  1,  1 },{  1,  1, -1 },{ -1,  1, -1 },

        { -1, -1, -1 },{  1, -1, -1 },{  1, -1,  1 },
        { -1, -1, -1 },{  1, -1,  1 },{ -1, -1,  1 },

        { -1, -1,  1 },{  1, -1,  1 },{  1,  1,  1 },
        { -1, -1,  1 },{  1,  1,  1 },{ -1,  1,  1 },

        {  1, -1, -1 },{ -1, -1, -1 },{ -1,  1, -1 },
        {  1, -1, -1 },{ -1,  1, -1 }, {  1,  1, -1 }
    };

    m_TexCoords = new RTRTexCoord_t[]{
        {0,0},{1,0},{1,1},
        {0,0},{1,1},{0,1},
        {0,0},{1,0},{1,1},
        {0,0},{1,1},{0,1},
        {0,0},{1,0},{1,1},
        {0,0},{1,1},{0,1},
        {0,0},{1,0},{1,1},
        {0,0},{1,1},{0,1},
        {0,0},{1,0},{1,1},
        {0,0},{1,1},{0,1},
        {0,0},{1,0},{1,1},
        {0,0},{1,1},{0,1}
    };

    m_Faces = new RTRFace_t[]{
        { 0, 1, 2 }, { 3, 4, 5 },   // +x
        { 6, 7, 8 }, { 9, 10, 11 },   // -x
        { 12, 13, 14 }, { 15, 16, 17 },   // +y
        { 18, 19, 20 }, { 21, 22, 23 },   // -y
        { 24, 25, 26 }, { 27, 28, 29 },   // +z
        { 30, 31, 32 }, { 33, 34, 35 }    // -z
    };
    RTRObject::Init();
}

//----------------------------------------------------------

void RTRPlane::Init()
{
    m_NumVertices = 4;
    m_NumTexCoords = 4;
    m_NumFaces = 2;
    m_VertexPoints = new RTRPoint_t[]{
        { -1, -1,  1 },
        {  1, -1,  1 },
        {  1,  1,  1 },
        { -1,  1,  1 }
    };
    m_TexCoords = new RTRTexCoord_t[]{
        {0,0},{1,0},{1,1},{0,1}
    };

    m_Faces = new RTRFace_t[]{
        { 0, 1, 2 }, { 0, 2, 3 }
    };
    RTRObject::Init();
}

//----------------------------------------------------------

RTRSphere::RTRSphere(float _radius, unsigned int _rings, unsigned int _sectors, glm::vec3 position, RTRMaterial_t* material) : RTRObject(material, position) {
    radius = _radius;
    rings = _rings;
    sectors = _sectors;
    m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(_radius, _radius, _radius));
}

void RTRSphere::Init() {
    tempPoints = std::vector<RTRPoint_t>();
    tempTexCoord = std::vector<RTRTexCoord_t>();
    tempIndices = std::vector<RTRFace_t>();

    //Calculating Vertecies
    for (int i = 0; i <= rings; i++) {
        float V = i / (float)rings;
        float phi = V * glm::pi<float>();

        for (int j = 0; j <= sectors; j++) {
            float U = j / (float)sectors;
            float theta = U * (glm::pi<float>() * 2);

            float x = cosf(theta) * sinf(phi);
            float y = cosf(phi);
            float z = sinf(theta) * sinf(phi);

            tempPoints.push_back(RTRPoint_t(x,y,z));
            tempTexCoord.push_back(RTRTexCoord_t(j % 2, i % 2));
            m_NumTexCoords++;
            m_NumVertices++;
        }

    }
    
    m_VertexPoints = &tempPoints[0];
    m_TexCoords = &tempTexCoord[0];

    for (int i = 0; i < sectors * rings + sectors; i++) {
        tempIndices.push_back(RTRFace_t(i, i + sectors + 1, i + sectors));
        tempIndices.push_back(RTRFace_t(i + sectors + 1, i, i + 1));
        m_NumFaces += 2;
    }
    m_Faces = &tempIndices[0];


    RTRObject::Init();

}


//----------------------------------------------------------

RTRCylinder::RTRCylinder(float _radius, float _height, unsigned int _sectors, glm::vec3 position, RTRMaterial_t* material, glm::vec3 rotation, float rotationAngleInRadians) : RTRObject(material, position) {
    radius = _radius;
    height = _height;
    sectors = _sectors;
    if (rotationAngleInRadians != 0) { m_ModelMatrix = glm::rotate(m_ModelMatrix, rotationAngleInRadians, rotation); }
    m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(_radius, _radius, _radius));
}

void RTRCylinder::Init() {
    tempPoints = std::vector<RTRPoint_t>();
    tempTexCoord = std::vector<RTRTexCoord_t>();
    tempIndices = std::vector<RTRFace_t>();

    //Calculating Vertecies
    for (int i = 0; i < 2; i++) {
        float y = i == 0 ? -height : height;

        for (int j = 0; j <= sectors; j++) {
            float U = j / (float)sectors;
            float theta = U * (glm::pi<float>() * 2);

            float x = cosf(theta);
            float z = sinf(theta);

            tempPoints.push_back(RTRPoint_t(x, y, z));
            tempTexCoord.push_back(RTRTexCoord_t(j % 2, i % 2));
            m_NumTexCoords++;
            m_NumVertices++;
        }

    }

    //add center points
    tempPoints.push_back(RTRPoint_t(0, -height, 0));
    tempPoints.push_back(RTRPoint_t(0, height, 0));
    tempTexCoord.push_back(RTRTexCoord_t(0.5, 1));
    tempTexCoord.push_back(RTRTexCoord_t(0.5, 0));
    m_NumTexCoords+=2;
    m_NumVertices+=2;

    m_VertexPoints = &tempPoints[0];
    m_TexCoords = &tempTexCoord[0];


    //add faces of sides
    for (int i = 0; i <= sectors; i++) {
        tempIndices.push_back(RTRFace_t(i, i + sectors, i + sectors + 1));
        tempIndices.push_back(RTRFace_t(i + sectors + 1, i + 1, i));
        m_NumFaces += 2;
    }

    //add top and bottom faces
    for (int i = 0; i <= sectors; i++) {
        tempIndices.push_back(RTRFace_t(m_NumVertices - 2, i, i + 1));
        tempIndices.push_back(RTRFace_t(m_NumVertices - 1, i + sectors + 1, i + sectors));
        m_NumFaces += 2;
    }

    m_Faces = &tempIndices[0];


    RTRObject::Init();

}

RTRTriangularPrism::RTRTriangularPrism(float _width, float _height, float _depth, glm::vec3 position, RTRMaterial_t* material, glm::vec3 rotation, float rotationAngleInRadians) : RTRObject(material, position)
{
    if (rotationAngleInRadians != 0) { m_ModelMatrix = glm::rotate(m_ModelMatrix, rotationAngleInRadians, rotation); }
    m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(_width, _height, _depth));
}

//default rotation is top left
void RTRTriangularPrism::Init()
{
    m_NumVertices = 24;
    m_NumTexCoords = 24;
    m_NumFaces = 8;

    m_VertexPoints = new RTRPoint_t[]{
        {  -1, 1,  1 },{  -1, -1, 1 },{  1,  1, 1 },    //+z
        {  -1, 1,  -1 },{  1, 1, -1 },{  -1,  -1, -1 }, //-z

        { -1, -1, -1 },{ -1, -1,  1 },{ -1,  1,  1 },
        { -1, -1, -1 },{ -1,  1,  1 },{ -1,  1, -1 },

        { -1,  1,  1 },{  1,  1,  1 },{  1,  1, -1 },
        { -1,  1,  1 },{  1,  1, -1 },{ -1,  1, -1 },

        {  1,  1,  1 },{  -1, -1,  1 },{  -1, -1, -1 },
        {  1,  1,  1 },{  -1, -1, -1 },{  1,  1,  -1 },
    };

    m_TexCoords = new RTRTexCoord_t[]{
        {0,1},{0,0},{1,1},
        {0,1},{0,0},{1,1},
        {0,0},{1,0},{1,1},
        {0,0},{1,1},{0,1},
        {0,0},{1,0},{1,1},
        {0,0},{1,1},{0,1},
        {0,0},{1,0},{1,1},
        {0,0},{1,1},{0,1},
    };

    m_Faces = new RTRFace_t[]{
        { 0, 1, 2 }, { 3, 4, 5 },   // front and back faces
        { 6, 7, 8 }, { 9, 10, 11 },   // -x
        { 12, 13, 14 }, { 15, 16, 17 },   // +y
        { 18, 19, 20 }, { 21, 22, 23 },   // inner face
    };
    RTRObject::Init();
}
