#pragma once

#include "Application.h"
#include "SystemObject.h"
#include "FlyCamera.h"
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "RenderTarget.h"
#include "Drawing.h"
#include <glm/mat4x4.hpp>


class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:


    SystemObject* sun;
    BaseCamera* camera;
    BaseCamera* mirrorCam;

    Drawing* m_advancedRendering;

    aie::ShaderProgram*  m_postShader;
    Mesh*                m_fullscreenQuad;
    aie::ShaderProgram*  m_phongShader;
    Mesh*                m_quadMesh;
    glm::mat4            m_quadTransform;

    aie::ShaderProgram*   m_advancedShader;
    aie::OBJMesh*         m_bunnyMesh;
    glm::mat4            m_bunnyTransform;

    aie::RenderTarget*   m_renderTarget;
    aie::RenderTarget*   m_postRender;

    Light                m_light;
    glm::vec3            m_ambientLight;
};