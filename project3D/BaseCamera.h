#pragma once
#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

class BaseCamera
{
public:
    BaseCamera();
    virtual ~BaseCamera();

    virtual void update(float deltatime) = 0;

    void setPerspective(float FOV, float aspectRatio, float near, float far) { m_projectionTransform = glm::perspective(FOV, aspectRatio, near, far); }
    void setLookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up) { m_viewTransform = glm::lookAt(from, to, up); }
    void setPosition(glm::vec3 pos) { m_worldTransform[3] = glm::vec4(pos.x, pos.y, pos.z, 0); }
    void setRotation(glm::vec3 rotation) { m_rotation = rotation; }

    glm::mat4 getWorldTransform() { return m_worldTransform; }
    glm::mat4 getView() { return m_viewTransform; }
    glm::mat4 getProjection() { return m_projectionTransform; }
    glm::mat4 getProjectionView() { return m_projectionViewTransform; }

    void updateProjectionView();

protected:

    glm::mat4 m_worldTransform;
    glm::mat4 m_viewTransform;
    glm::mat4 m_projectionTransform;
    glm::mat4 m_projectionViewTransform;
    glm::vec3 m_rotation;

};

