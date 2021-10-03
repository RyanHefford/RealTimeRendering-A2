//-----------------------------------------------------------------------------
// RTRObject.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include "RTRLighting.h"
#include "RTRShader.h"
#include "RTRTexture.h"

struct RTRPoint_t {
    float x, y, z;
};

struct RTRFace_t {
    unsigned int v0, v1, v2;
};

struct RTRTexCoord_t {
    float s, t;
};

//-----------------------------------------------------------------------------
class RTRObject
{
public:
    RTRObject(unsigned int texId, glm::vec3 position = { 0,0,0 });
    ~RTRObject() {}
    virtual void Init();
    virtual void Render(RTRShader* shader);
    virtual void End();
    void SetMaterial(RTRMaterial_t mat) { m_Material = mat; }
    virtual const char* GetName() { return "RTRObject"; }

public:
    unsigned int m_NumVertices{ 0 };
    unsigned int m_NumFaces{ 0 };
    unsigned int m_NumTexCoords{ 0 };
    RTRPoint_t* m_VertexPoints{ nullptr };
    RTRFace_t* m_Faces{ nullptr };
    RTRTexCoord_t* m_TexCoords{ nullptr };
    RTRMaterial_t m_Material{ {0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 }, 128.0 };//{0.19225, 0.19225, 0.19225 }, { 0.50754, 0.50754, 0.50754 }, { 0.508273, 0.508273, 0.508273 }, 128.0 };
    unsigned int m_TextureId{ 0 };
    unsigned int m_VertexBuffer{ 0 };
    unsigned int m_VertexArray{ 0 };
    unsigned int m_TexCoordBuffer{ 0 };
    unsigned int m_FaceElementBuffer{ 0 };
    glm::mat4 m_ModelMatrix = glm::mat4(1);
};


//-----------------------------------------------------------------------------
class RTRRectangle : public RTRObject
{
public:
    RTRRectangle(float _width, float _height, float _depth, glm::vec3 position, unsigned int texId, glm::vec3 rotation = glm::vec3(0), float rotationAngle = 0);

    ~RTRRectangle() {}
    virtual void Init();
    virtual const char* GetName() { return "RTRRectangle"; }

};


//-----------------------------------------------------------------------------
class RTRCube : public RTRObject
{
public:
    RTRCube(glm::vec3 position, unsigned int texId) : RTRObject(texId, position) {}
    ~RTRCube() {}
    virtual void Init();
    virtual const char* GetName() { return "RTRCube"; }
};

//-----------------------------------------------------------------------------
class RTRPlane : public RTRObject
{
public:
    RTRPlane(glm::vec3 position, unsigned int texId) : RTRObject(texId, position) {}

    ~RTRPlane() {}
    virtual void Init();
    virtual const char* GetName() { return "RTRPlane"; }

};