//-----------------------------------------------------------------------------
// RTRObject.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include "RTRLighting.h"
#include "RTRShader.h"
#include "RTRTexture.h"
#include <vector>

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
    RTRObject(RTRMaterial_t* material, glm::vec3 position = { 0,0,0 });
    ~RTRObject() {}
    virtual void Init();
    virtual void Render(RTRShader* shader);
    virtual void End();
    void SetMaterial(RTRMaterial_t* mat) { m_Material = mat; }
    virtual const char* GetName() { return "RTRObject"; }

public:
    unsigned int m_NumVertices{ 0 };
    unsigned int m_NumFaces{ 0 };
    unsigned int m_NumTexCoords{ 0 };
    RTRPoint_t* m_VertexPoints{ nullptr };
    RTRFace_t* m_Faces{ nullptr };
    RTRTexCoord_t* m_TexCoords{ nullptr };
    RTRMaterial_t* m_Material;
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
    RTRRectangle(float _width, float _height, float _depth, glm::vec3 position, RTRMaterial_t* material, glm::vec3 rotation = glm::vec3(0), float rotationAngleInRadians = 0);

    ~RTRRectangle() {}
    virtual void Init();
    virtual const char* GetName() { return "RTRRectangle"; }

};


//-----------------------------------------------------------------------------
class RTRCube : public RTRRectangle
{
public:
    RTRCube(float scale, glm::vec3 position, RTRMaterial_t* material) : RTRRectangle(scale, scale, scale, position, material) {}
    ~RTRCube() {}
    virtual void Init() { RTRRectangle::Init(); }
    virtual const char* GetName() { return "RTRCube"; }
};

//-----------------------------------------------------------------------------
class RTRTriangularPrism : public RTRObject
{
public:
    RTRTriangularPrism(float _width, float _height, float _depth, glm::vec3 position, RTRMaterial_t* material, glm::vec3 rotation = glm::vec3(0), float rotationAngleInRadians = 0);
    ~RTRTriangularPrism() {}
    virtual void Init();
    virtual const char* GetName() { return "RTRCube"; }
};

//-----------------------------------------------------------------------------
class RTRPlane : public RTRObject
{
public:
    RTRPlane(glm::vec3 position, RTRMaterial_t* material) : RTRObject(material, position) {}

    ~RTRPlane() {}
    virtual void Init();
    virtual const char* GetName() { return "RTRPlane"; }

};

//-----------------------------------------------------------------------------
class RTRSphere : public RTRObject
{
public:
    RTRSphere(float _radius, unsigned int _rings, unsigned int _sectors, glm::vec3 position, RTRMaterial_t* material);

    ~RTRSphere() {}
    virtual void Init();
    virtual const char* GetName() { return "RTRSphere"; }

private:
    std::vector<RTRPoint_t> tempPoints;
    std::vector<RTRTexCoord_t> tempTexCoord;
    std::vector<RTRFace_t> tempIndices;

    float radius;
    unsigned int rings;
    unsigned int sectors;
};


//-----------------------------------------------------------------------------
class RTRCylinder : public RTRObject
{
public:
    RTRCylinder(float _radius, float _height, unsigned int _sectors, glm::vec3 position, RTRMaterial_t* material, glm::vec3 rotation = glm::vec3(0), float rotationAngleInRadians = 0);

    ~RTRCylinder() {}
    virtual void Init();
    virtual const char* GetName() { return "RTRCylinder"; }

private:
    std::vector<RTRPoint_t> tempPoints;
    std::vector<RTRTexCoord_t> tempTexCoord;
    std::vector<RTRFace_t> tempIndices;

    float radius;
    float height;
    unsigned int sectors;
};
