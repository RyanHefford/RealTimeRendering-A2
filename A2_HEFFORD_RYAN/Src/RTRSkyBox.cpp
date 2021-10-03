#include "RTRSkyBox.h"

RTRSkyBox::RTRSkyBox(unsigned int texId) : RTRObject(texId)
{

    m_NumVertices = 8;
    m_NumTexCoords = 8;
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
    m_Faces = new RTRFace_t[]{
        { 1, 7, 4 }, { 1, 2, 7 },   // +x
        { 5, 3, 0 }, { 5, 6, 3 },   // -x
        { 3, 7, 2 }, { 3, 6, 7 },   // +y
        { 5, 1, 4 }, { 5, 0, 1 },   // -y
        { 0, 2, 1 }, { 0, 3, 2 },   // +z
        { 4, 6, 5 }, { 4, 7, 6 }    // -z
    };

    Init();
}

void RTRSkyBox::Init()
{
    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);

    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_NumVertices * sizeof(RTRPoint_t), m_VertexPoints, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RTRPoint_t), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_FaceElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_FaceElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NumFaces * sizeof(RTRFace_t), m_Faces, GL_STATIC_DRAW);
}

void RTRSkyBox::Render(RTRShader* shader)
{
    glDepthFunc(GL_LEQUAL);
    shader->SetInt("u_SkyboxTextureId", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureId);
    glBindVertexArray(m_VertexArray);
    glDrawElements(GL_TRIANGLES, 3 * m_NumFaces, GL_UNSIGNED_INT, 0);
    glDepthFunc(GL_LESS);
    glBindVertexArray(0);
}

void RTRSkyBox::End()
{
    glDeleteVertexArrays(1, &m_VertexArray); m_VertexArray = 0;
    glDeleteBuffers(1, &m_VertexBuffer); m_VertexBuffer = 0;
    glDeleteBuffers(1, &m_FaceElementBuffer); m_FaceElementBuffer = 0;
    if (m_VertexPoints != nullptr) { delete m_VertexPoints; m_VertexPoints = nullptr; }
    if (m_Faces != nullptr) { delete m_Faces; m_Faces = nullptr; }
}