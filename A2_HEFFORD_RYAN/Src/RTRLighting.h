//-----------------------------------------------------------------------------
// RTRLighting.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include <glm/glm.hpp>
#include "RTRTexture.h"
#include "RTRCubeMapTexture.h"

constexpr auto RTR_MAX_LIGHTS = 10;

enum class RTRLightType : int
{
    DirectionalLight = 0,
    PointLight,
    SpotLight
};

enum class RTRMaterialType : int
{
    BasicMaterial = 0,
    TexturedMaterial,
    ReflectiveMaterial,
    GlassMaterial,
    SkyboxMaterial
};

// TODO: Support for spot lights
struct RTRLight_t
{
    RTRLightType Type;      // Used by light type ...
    glm::vec3 Ambient;      //   Directional, Point, Spot
    glm::vec3 Diffuse;      //   Directional, Point, Spot
    glm::vec3 Specular;     //   Directional, Point, Spot
    glm::vec3 Direction;    //   Directional, Spot
    glm::vec3 Position;     //   Point, Spot
    float Constant;         //   Point, Spot
    float Linear;           //   Point, Spot
    float Quadratic;        //   Point, Spot
    float Cutoff;           //   Spot
    float OuterCutoff;      //   Spot
};

struct RTRMaterial_t
{
    RTRMaterialType Type;           // Used by material type
    glm::vec3 Ambient;              // Basic, Textured, Reflective, Glass
    glm::vec3 Diffuse;              // Basic, Textured, Reflective, Glass
    glm::vec3 Specular;             // Basic, Textured, Reflective, Glass
    float Shininess;                // Basic, Textured, Reflective, Glass
    RTRTexture* DiffuseMap;        // Textured, Reflective, Glass
    RTRTexture* SpecularMap;       // Textured, Reflective, Glass
    RTRTexture* ReflectionMap;     // Reflective, Glass
    float RefractiveIndex;          // Glass
    RTRCubeMapTexture* CubeMap;
};


class RTRLightingModel
{
public:
    RTRLightingModel() {}
    ~RTRLightingModel() {}
    int GetNumLights() { return m_NumLights; }
    RTRLight_t* GetLight(int i) { return &m_Lights[i]; }
    int AddLight(RTRLight_t light) {
        if (m_NumLights == RTR_MAX_LIGHTS) return -1;
        m_Lights[m_NumLights] = light;
        m_NumLights++;
        return 0;
    }

protected:
    int m_NumLights{ 0 };
    RTRLight_t m_Lights[RTR_MAX_LIGHTS]{ };
};
