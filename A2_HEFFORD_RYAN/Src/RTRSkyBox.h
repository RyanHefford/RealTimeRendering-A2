#pragma once

#include "RTRObject.h"

class RTRSkyBox : RTRObject{
public:
    RTRSkyBox(unsigned int texId);
    ~RTRSkyBox() {}
    virtual void Init();
    virtual void Render(RTRShader* shader);
    virtual void End();
    virtual const char* GetName() { return "RTRSkyBox"; }

    RTRPoint_t* m_TexCoords{ nullptr };
};