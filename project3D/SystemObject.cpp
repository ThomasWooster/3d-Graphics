#include "SystemObject.h"
#include <iostream>



SystemObject::SystemObject(std::string name, float radius, float distFromParent, glm::vec4 colour, SystemObject* parent)
{
    children = std::vector<SystemObject*>();
    m_name = name;
    m_radius = radius;
    m_distFromParent = distFromParent;
    m_colour = colour;
    if (parent != nullptr)
    {
        m_parent = parent;

        parent->setChild(this);
    }
    else
    {
        m_parent = nullptr;
    }

    m_localTransform = glm::mat4(1);
    m_globalTransform = glm::mat4(1);
}


SystemObject::~SystemObject()
{
}


void SystemObject::setChild(SystemObject* child)
{
    children.push_back(child);
}

void SystemObject::Update(float time)
{
    float rotationSpeed = 0;

    //calculate speed
    if (m_distFromParent != 0)
    {
        rotationSpeed = pow(5 / m_distFromParent, 0.5f);
    }

    float rotationAngle = rotationSpeed * time;

    //std::cout << rotationSpeed << std::endl;

    //create local tranform
    m_localTransform = glm::mat4(1);

    m_localTransform[3] = glm::vec4({ m_distFromParent, 0, 0, 1 });

    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

    m_localTransform = rotationMatrix * m_localTransform;

    if (m_parent != nullptr)
    {
        m_globalTransform = m_parent->m_globalTransform * m_localTransform;
    }
    


    aie::Gizmos::addSphere(m_globalTransform[3], m_radius, 25, 20, m_colour);

    for (size_t i = 0; i < children.size(); i++)
    {
        children[i]->Update(time);
    }
}