#pragma once
#include "BaseCamera.h"

class MirrorCamera : public BaseCamera
{
public:
    MirrorCamera();
    virtual ~MirrorCamera();

    virtual void update(float deltatime);

};

