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

RTRObject::RTRObject(glm::vec3 position, unsigned int texId)
{
    m_ModelMatrix = glm::translate(m_ModelMatrix, position);
    m_TextureId = texId;
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
    shader->SetMaterial("u_ObjectMaterial", m_Material);
    shader->SetInt("u_ObjectMaterial.Diffuse", 0);
    glActiveTexture(GL_TEXTURE0);// <------select the Texture Unit to bind this texture to
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
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

//-----------------------------------------------------------------------------
void RTRCube::Init()
{
    m_NumVertices = 8;
    m_NumTexCoords = 36;
    m_NumFaces = 12;
    m_VertexPoints = new RTRPoint_t[]{
        { -1, -1,  1 },
        {  1, -1,  1 },
        {  1,  1,  1 },
        { -1,  1,  1 },
        {  1, -1, -1 },
        { -1, -1, -1 },
        { -1,  1, -1 },
        {  1,  1, -1 }
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
        { 1, 4, 7 }, { 1, 7, 2 },   // +x
        { 5, 0, 3 }, { 5, 3, 6 },   // -x
        { 3, 2, 7 }, { 3, 7, 6 },   // +y
        { 5, 4, 1 }, { 5, 1, 0 },   // -y
        { 0, 1, 2 }, { 0, 2, 3 },   // +z
        { 4, 5, 6 }, { 4, 6, 7 }    // -z
    };
    RTRObject::Init();
}


RTRRectangle::RTRRectangle(float _width, float _height, float _depth, glm::vec3 position, unsigned int texId, glm::vec3 rotation, float rotationAngleInRadians) : RTRObject(position, texId)
{
    if (rotationAngleInRadians != 0) { m_ModelMatrix = glm::rotate(m_ModelMatrix, rotationAngleInRadians, rotation); }
    m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(_width, _height, _depth));
}

void RTRRectangle::Init()
{
    m_NumVertices = 8;
    m_NumTexCoords = 36;
    m_NumFaces = 12;
    m_VertexPoints = new RTRPoint_t[]{
        { -1, -1,  1 },
        {  1, -1,  1 },
        {  1,  1,  1 },
        { -1,  1,  1 },
        {  1, -1, -1 },
        { -1, -1, -1 },
        { -1,  1, -1 },
        {  1,  1, -1 }
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
        { 1, 4, 7 }, { 1, 7, 2 },   // +x
        { 5, 0, 3 }, { 5, 3, 6 },   // -x
        { 3, 2, 7 }, { 3, 7, 6 },   // +y
        { 5, 4, 1 }, { 5, 1, 0 },   // -y
        { 0, 1, 2 }, { 0, 2, 3 },   // +z
        { 4, 5, 6 }, { 4, 6, 7 }    // -z
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
