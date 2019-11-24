#pragma once
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "MirrorCamera.h"
#include "FlyCamera.h"
#include "OBJObject.h"
#include <vector>
#include <string>
struct Light
{
    glm::vec3 direction;
    glm::vec3 diffuse;
    glm::vec3 specular;
};


class Drawing
{
public:
    Drawing(std::string frag, std::string vert);
    virtual ~Drawing();

	void addObject(OBJObject* gameObject);
    void Draw(BaseCamera* camera, Light light, glm::vec3 ambientLight);
    void Clear();

private:
    aie::ShaderProgram* m_shader;

    std::vector<OBJObject*> m_Objects;
};

