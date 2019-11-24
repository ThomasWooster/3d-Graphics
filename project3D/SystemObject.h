#pragma once


#include <vector>
#include <glm/ext.hpp>
#include <string.h>
#include <Gizmos.h>

class SystemObject
{
public:
    SystemObject(std::string name, float radius, float distFromParent, glm::vec4 colour, SystemObject* parent = nullptr);
    virtual ~SystemObject();

    void setChild(SystemObject* child);
    void Update(float time);


    SystemObject* m_parent;
    std::vector<SystemObject*> children;
    std::string m_name;
    float m_radius;
    float m_distFromParent;
    glm::vec4 m_colour;
    glm::mat4 m_localTransform;
    glm::mat4 m_globalTransform;

private:
  
};

