#include "BaseCamera.h"



BaseCamera::BaseCamera()
{
    m_projectionTransform = glm::mat4(1);
    m_projectionViewTransform = glm::mat4(1);
    m_viewTransform = glm::mat4(1);
    m_worldTransform = glm::mat4(1);
    m_rotation = glm::vec3(0, 0, 0);
}


BaseCamera::~BaseCamera()
{
}

void BaseCamera::updateProjectionView()
{
    m_projectionViewTransform = m_projectionTransform * m_viewTransform;
}