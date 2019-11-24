#include "Drawing.h"



Drawing::Drawing(std::string frag, std::string vert)
{
    m_shader = new aie::ShaderProgram();

    std::string Vert = "./shaders/" + vert + ".vert";
    std::string Frag = "./shaders/" + frag + ".frag";

    m_shader->loadShader(aie::eShaderStage::VERTEX, Vert.c_str());
    m_shader->loadShader(aie::eShaderStage::FRAGMENT, Frag.c_str());

    if (m_shader->link() == false)
    {
        printf("Shader Error: %s\n", m_shader->getLastError());
    }
}


Drawing::~Drawing()
{
    delete(m_shader); 
}

void Drawing::addObject(OBJObject* gameObject)
{
	m_Objects.push_back(gameObject);
}

void Drawing::Draw(BaseCamera* camera, Light light, glm::vec3 ambientLight)
{
    for (size_t i = 0; i < m_Objects.size(); i++)
    {
        // bind the camera location
        m_shader->bindUniform("cameraPosition", camera->getWorldTransform()[3]);

        //bind shader
        m_shader->bind();

        //bind transform
        glm::mat4 pvm = camera->getProjection() * camera->getView() * m_Objects[i]->getTransform();
        m_shader->bindUniform("ProjectionViewModel", pvm);

        //bind transforms for lighting
		m_shader->bindUniform("NormalMatrix", glm::mat3(m_Objects[i]->getTransform()));

        // bind light
        m_shader->bindUniform("Ia", ambientLight);
        m_shader->bindUniform("Id", light.diffuse);
        m_shader->bindUniform("Is", light.specular);
        m_shader->bindUniform("lightDirection", light.direction);
        m_shader->bindUniform("roughness", 0.1f);
        m_shader->bindUniform("reflectionCoefficient", 1.0f);

        //bind texture to a specific location
        m_Objects[i]->getOBJMesh()->draw();
    }
}

void Drawing::Clear()
{
	m_Objects.clear();
}
