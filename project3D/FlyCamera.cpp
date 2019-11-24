#include "FlyCamera.h"
#include <Input.h>
#include <iostream>



FlyCamera::FlyCamera() : BaseCamera::BaseCamera()
{
    m_up = glm::vec3(0, 1, 0);
    m_speed = 10.0f;
}


FlyCamera::~FlyCamera()
{
}


void FlyCamera::update(float deltatime)
{
    aie::Input* input = aie::Input::getInstance();


    if (input->isKeyDown(aie::INPUT_KEY_UP))
    {
        m_rotation.x -= m_speed * deltatime * 0.2f;
    }
    if (input->isKeyDown(aie::INPUT_KEY_DOWN))
    {
        m_rotation.x += m_speed * deltatime* 0.2f;
    }
    if (input->isKeyDown(aie::INPUT_KEY_LEFT))
    {
        m_rotation.y += m_speed * deltatime * 0.2f;
    }
    if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
    {
        m_rotation.y -= m_speed * deltatime * 0.2f;
    }

    if (input->isKeyDown(aie::INPUT_KEY_W))
    {
        m_worldTransform[3] += m_worldTransform[2] * m_speed * deltatime;
    }
    if (input->isKeyDown(aie::INPUT_KEY_S))
    {
        m_worldTransform[3] -= m_worldTransform[2] * m_speed * deltatime;
    }
    if (input->isKeyDown(aie::INPUT_KEY_A))
    {
        m_worldTransform[3] += m_worldTransform[0] * m_speed * deltatime;
    }
    if (input->isKeyDown(aie::INPUT_KEY_D))
    {
        m_worldTransform[3] -= m_worldTransform[0] * m_speed * deltatime;
    }

    glm::quat newR(m_rotation);
    glm::vec4 pos = m_worldTransform[3];

    m_worldTransform = glm::mat4(newR);
    m_worldTransform[3] = pos;


    setLookAt(m_worldTransform[3], m_worldTransform[2] + m_worldTransform[3], glm::vec3(0, 1, 0));

    updateProjectionView();
    
}