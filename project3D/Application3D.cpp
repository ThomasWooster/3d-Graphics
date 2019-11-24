 #include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include "SystemObject.h"
#include "MirrorCamera.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Application3D::Application3D() 
{

}

Application3D::~Application3D() 
{

}

bool Application3D::startup() 
{
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

    /*
    sun = new SystemObject("Sun", 2.0f, 0.0f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), nullptr);
    SystemObject* earth = new SystemObject("Earth", 0.6f, 3.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), sun);
    SystemObject* mars = new SystemObject("Mars", 0.5f, 5.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), sun);
    SystemObject* phobos = new SystemObject("Phobos", 0.2f, 0.9f, glm::vec4(1.0f, 0.3f, 0.0f, 1.0f), mars);
    SystemObject* pluto = new SystemObject("Pluto", 0.2f, 12.0f, glm::vec4(0.0f, 1.0f, 0.5f, 1.0f), sun);
    */

    camera = new FlyCamera();
    mirrorCam = new MirrorCamera();

    camera->setLookAt(vec3(10), vec3(0), vec3(0, 1, 0));
    camera->setPerspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.f);
    camera->setRotation(vec3(0, 0, 0));
	// create simple camera transforms

    mirrorCam->setPerspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.f);
    mirrorCam->setPosition(vec3(0, 0, 0));
    mirrorCam->setLookAt(mirrorCam->getWorldTransform()[3], mirrorCam->getWorldTransform()[2] + mirrorCam->getWorldTransform()[3], vec3(0, 1, 0));
    mirrorCam->setRotation(vec3(-1.51, 0, 0));

    ///////////////////////////////////////////////////////////////////
    m_advancedRendering = new Drawing("advancedstuff", "normalmap");
    m_advancedShader = new aie::ShaderProgram();
    m_bunnyMesh = new aie::OBJMesh();

    m_phongShader = new aie::ShaderProgram();
    m_quadMesh = new Mesh();

    m_postShader = new aie::ShaderProgram();
    m_fullscreenQuad = new Mesh();

    m_phongShader->loadShader(aie::eShaderStage::VERTEX, "./shaders/textured.vert");
    m_phongShader->loadShader(aie::eShaderStage::FRAGMENT, "./shaders/textured.frag");
    m_advancedShader->loadShader(aie::eShaderStage::VERTEX, "./shaders/normalmap.vert");
    m_advancedShader->loadShader(aie::eShaderStage::VERTEX, "./shaders/advancedstuff.frag");
    m_postShader->loadShader(aie::eShaderStage::VERTEX, "./shaders/post.vert");
    m_postShader->loadShader(aie::eShaderStage::FRAGMENT, "./shaders/post.frag");


    if (m_phongShader->link() == false)
    {
        printf("Shader Error: %s\n", m_phongShader->getLastError());
        return false;
    }
    if (m_advancedShader->link() == false)
    {
        printf("Shader Error: %s\n", m_advancedShader->getLastError());
        return false;
    }
    if (m_postShader->link() == false)
    {
        printf("Shader Error: %s\n", m_postShader->getLastError());
        return false;
    }

    m_renderTarget = new aie::RenderTarget();
    m_postRender = new aie::RenderTarget();

    if (m_renderTarget->initialise(1, getWindowWidth(), getWindowHeight()) == false)
    {
        printf("Render Target Error!\n");
        return false;
    }
    if (m_postRender->initialise(1, getWindowWidth(), getWindowHeight()) == false)
    {
        printf("Render Target Error!\n");
        return false;
    }

    m_quadMesh->initiliseQuad();
    m_fullscreenQuad->initialiseFullscreenQuad();

    //define scale matrix fore the qud
    m_quadTransform = {
        10, 0, 0, 0,
        0, 10, 0, 0,
        0, 0, 10, 0,
        0, 0, 0, 1,
    };

    if (m_bunnyMesh->load("./Models/soulspear/soulspear.obj", true, true) == false)
    {
        printf("Soulspear Mesh Error!\n");
        return false;
    }

    m_bunnyTransform = {
       0.5f, 0, 0, 0,
       0, 0.5f, 0, 0,
       0, 0, 0.5f, 0,
       0, 0, 0, 1
    };

    // set up lighting
    m_light.diffuse = { 1, 1, 1 };
    m_light.specular = { 1, 1 , 1 };
    m_ambientLight = { 0.5f, 0.5f, 0.5f };

    m_light.direction = glm::vec3(0, 0, -1);
    

	return true;
}

void Application3D::shutdown() 
{

	Gizmos::destroy();
}

void Application3D::update(float deltaTime) 
{

	// query time since application started
	float time = getTime();

    // rotate light
    

    camera->update(deltaTime);
    mirrorCam->update(deltaTime);

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

    
    m_advancedRendering->Clear();

    m_advancedRendering->addObject(new OBJObject(m_bunnyTransform, m_bunnyMesh));


    //sun->Update(time);


    glm::vec4 pos = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    m_bunnyTransform[3] = pos;

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application3D::draw()
{
    
    //bind our render target
    m_renderTarget->bind();

    // wipe the screen to the background colour
    clearScreen();
    mirrorCam->setPerspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

    m_advancedRendering->Draw(mirrorCam, m_light, m_ambientLight);

    // unbind target to return to the back buffer
    m_renderTarget->unbind();

    // clear the back buffer
    clearScreen();


    //////////////////////////////////////////
    m_postRender->bind();

    clearScreen();
    camera->setPerspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

    m_advancedRendering->Draw(camera, m_light, m_ambientLight);

    //bind textured shader
    m_phongShader->bind();
    glm::mat4 pvm = camera->getProjection() * camera->getView() * m_quadTransform;
    m_phongShader->bindUniform("ProjectionViewModel", pvm);
    m_phongShader->bindUniform("diffuseTexture", 0);
    m_renderTarget->getTarget(0).bind(0);
    //draw quad
    m_quadMesh->draw();


    m_postRender->unbind();

    clearScreen();

   

    //////////////////////////////////////////  

    m_postShader->bind();
    m_postShader->bindUniform("colourTarget", 0);
    m_postRender->getTarget(0).bind(0);

    m_fullscreenQuad->draw();




    Gizmos::draw(camera->getProjectionView());
    Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}