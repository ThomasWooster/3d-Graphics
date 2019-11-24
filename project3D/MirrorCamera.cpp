#include "MirrorCamera.h"

MirrorCamera::MirrorCamera()
{

}

MirrorCamera::~MirrorCamera()
{

}

void MirrorCamera::update(float deltatime)
{
    glm::quat newR(m_rotation);

    m_worldTransform = glm::mat4(newR);

    setLookAt(m_worldTransform[3], m_worldTransform[2] + m_worldTransform[3], glm::vec3(0, 1, 0));

    updateProjectionView();
}