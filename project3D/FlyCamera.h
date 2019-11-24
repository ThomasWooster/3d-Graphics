#pragma once

#include "BaseCamera.h"

class FlyCamera : public BaseCamera
{
public:
    FlyCamera();
    virtual ~FlyCamera();

    virtual void update(float deltatime);

    void setSpeed(float speed) { m_speed = speed; }

protected:
    float m_speed;
    glm::vec3 m_up;
};

