#include <Renderer.h>
#include <QOpenGLContext>
#include <QQuickItem>
#include <SimpleVT.h>
#include <geGL/geGL.h>
#include <geCore/Text.h>
#include <memory>
#include <geUtil/OrbitCamera.h>
#include <geUtil/PerspectiveCamera.h>
#include <glsg/GLSceneProcessor.h>
#include <QtImageLoader.h>
#include <iostream>
#include <sg/CubeMapTexture.h>
#include <sg/AnimationManager.h>
#include <glsg/TextureFactory.h>
#include <geSG/Material.h>

#include <skybox/SkyboxVT.h>
#include <Effects/laser/LaserVT.h>
#include <LaserManager.h>


msg::Renderer::Renderer(QObject *parent) :
    QuickRendererBase(parent),
    orbitCamera(std::make_shared<ge::util::OrbitCamera>()),
    perspectiveCamera(std::make_shared<ge::util::PerspectiveCamera>()),
    _gl(nullptr),
    _scene(nullptr),
    _glScene(nullptr),
    _time(std::make_shared<double>()),
    _sceneToProcess(false),
    _viewPort(std::make_shared<glm::vec2>(1000,800)),
    _simpleVT(std::make_shared<msg::SimpleVT>()),
    _skyboxVT(std::make_shared<msg::SkyboxVT>()),
    _laserVT(std::make_shared<msg::LaserVT>()),
    _animationManager(std::make_shared<msg::AnimationManager>()),
    _clock(std::chrono::high_resolution_clock::now()),
    _laserManager(std::make_shared<msg::LaserManager>())
{
    std::cout << "Renderer ctor" << std::endl;
    setupCamera();
}


void msg::Renderer::onViewportChange() {
    std::cout << "onViewportChange" << std::endl;
    _gl->glViewport(0, 0, _viewPort->x, _viewPort->y);
    perspectiveCamera->setAspect(_viewPort->x / _viewPort->y);
}

void msg::Renderer::setupGLState() {
    //std::cout << "Renderer setupGLState" << std::endl;
    _gl->glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    _gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _gl->glEnable(GL_DEPTH_TEST);
}

void msg::Renderer::onWidthChanged(int w) {
    std::cout << "Rendered onWidthChanged" << std::endl;
    _viewPort->x = w;
    onViewportChange();
}

void msg::Renderer::onHeightChanged(int h) {
    std::cout << "Rendered onHeightChanged" << std::endl;
    _viewPort->y = h;
    onViewportChange();
}

void msg::Renderer::beforeRendering() {
    //std::cout << "Renderer beforeRendering" << std::endl;
    update();
    setupGLState();
    drawVT();
    _qqw->resetOpenGLState();
    _qqw->update();
}

void msg::Renderer::onOGLContextCreated(QOpenGLContext *context) {
    std::cout << "Renderer onOGLContextCreated" << std::endl;
    context->makeCurrent(_qqw);

    ge::gl::init();
    _gl = std::make_shared<ge::gl::Context>();
    initVT();

    context->doneCurrent();
}

void msg::Renderer::setScene(std::shared_ptr<ge::sg::Scene> &loadedScene) {
    std::cout << "Renderer setScene" << std::endl;
    if ( loadedScene == _scene) return;
    _scene = loadedScene;
    _sceneToProcess = true;
}

void msg::Renderer::update() {
    if (_sceneToProcess) {
        std::cout << "Scene Processing" << std::endl;
        _glScene = ge::glsg::GLSceneProcessor::processScene(_scene,_gl);
        _simpleVT->setScene(_glScene);
        _simpleVT->processScene();
        _sceneToProcess = false;

        _laserManager->orbitCamera = orbitCamera;
        std::shared_ptr<msg::Laser> l(
            std::make_shared<msg::Laser>(
                glm::vec3(0.f,0.f,8.f),
                glm::vec3(0.f,0.f,15.f),
                glm::vec4(0.f,0.f,1.f,1.f),
                2.0f
            )
        );
        _laserManager->addMissile(l);
        _laserVT->lasers = _laserManager->missiles;
    }

    _laserManager->update();
    _laserVT->update();
    auto projectionMatrix = perspectiveCamera->getProjection();
    auto viewMatrix = orbitCamera->getView();
    _simpleVT->program->setMatrix4fv("projectionMatrix", glm::value_ptr(projectionMatrix));
    _simpleVT->program->setMatrix4fv("viewMatrix", glm::value_ptr(viewMatrix));


    glm::vec3 CP = static_cast<glm::vec3>(glm::inverse(viewMatrix)[3]);
    glm::vec4 WP(-_viewPort->x * 0.5f, -_viewPort->y * 0.5f, _viewPort->x, _viewPort->y);
    glm::mat4 OP = glm::ortho(-_viewPort->x * 0.5f, _viewPort->x * 0.5f, -_viewPort->y * 0.5f, _viewPort->y * 0.5f, -1.0f, 1.0f);
    glm::mat4 OW = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 1.0f, 0.0));

    _laserVT->program->set4fv("viewport", glm::value_ptr(WP));
    _laserVT->program->set3fv("_cameraPosition", glm::value_ptr(CP));
    _laserVT->program->setMatrix4fv("u_View", glm::value_ptr(OW));
    _laserVT->program->setMatrix4fv("u_Projection", glm::value_ptr(OP));
    _laserVT->program->setMatrix4fv("projectionMatrix", glm::value_ptr(projectionMatrix));
    _laserVT->program->setMatrix4fv("viewMatrix", glm::value_ptr(viewMatrix));

}

void viewPortResize(int w, int h) {
 // TODO 
}

void msg::Renderer::setupCamera() {
    std::cout << "Renderer setupCamera" << std::endl;
    orbitCamera->setDistance(20.f);
    orbitCamera->setYAngle(3.1415f/1.2f);
    orbitCamera->setXAngle(3.1415f/1.0f);
    orbitCamera->setFocus(glm::vec3(0,0,0));

    perspectiveCamera->setNear(0.1f);
    perspectiveCamera->setFar(5000.0f);
    perspectiveCamera->setFovy(45.0f);
    perspectiveCamera->setAspect(1000.0f / 800.0f);
}


bool msg::Renderer::initSimpleVT() {
    std::cout << "Renderer initSimpleVT" << std::endl;

    std::string shaderDir(APP_RESOURCES"/shaders/");
    _simpleVT->gl = _gl;

    auto simple_vs(std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, ge::core::loadTextFile(shaderDir+"simple_vs.glsl")));
    auto simple_fs(std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, ge::core::loadTextFile(shaderDir+"simple_fs.glsl")));
    auto prog(std::make_shared<ge::gl::Program>(simple_vs, simple_fs));
    if (!prog.get()->isProgram()) {
        std::cerr << "Problem with simpleVT shaders" << std::endl;
    }
    _simpleVT->program = prog;
    return true;
}

bool msg::Renderer::initLaserVT() {
    std::cout << "Renderer initLaserVT" << std::endl;

    std::string shaderDir(APP_RESOURCES"/shaders/");
    auto laser_vs(std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, ge::core::loadTextFile(shaderDir+"laser_vs.glsl")));
    auto laser_fs(std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, ge::core::loadTextFile(shaderDir+"laser_fs.glsl")));
    auto laser_gs(std::make_shared<ge::gl::Shader>(GL_GEOMETRY_SHADER, ge::core::loadTextFile(shaderDir+"laser_gs.glsl")));
    auto program(std::make_shared<ge::gl::Program>(laser_vs, laser_fs, laser_gs));
    
    _laserVT->gl = _gl;
    _laserVT->program = program;

    std::string imagePath(APP_RESOURCES"/texture/laserbolt.png");
    std::shared_ptr<QtImage> image(QtImageLoader::loadImage(imagePath.c_str()));

    auto materialComponent(std::make_shared<ge::sg::MaterialImageComponent>());
    materialComponent->semantic = ge::sg::MaterialImageComponent::Semantic::diffuseTexture;
    materialComponent->filePath = imagePath;
    materialComponent->image = image;
    materialComponent->setType(ge::sg::MaterialComponent::ComponentType::IMAGE);

    std::shared_ptr<ge::glsg::TextureFactory> textureFactory(std::make_shared<ge::glsg::DefaultTextureFactory>());
    _laserVT->texture = textureFactory->create(materialComponent.get(), _gl);
    return true;
}

bool msg::Renderer::initSkyboxVT() {
    std::cout << "Renderer initSkyboxVT" << std::endl;

    std::string shaderDir(APP_RESOURCES"/shaders/");
    auto skybox_vs(std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, ge::core::loadTextFile(shaderDir+"skybox_vs.glsl")));
    auto skybox_gs(std::make_shared<ge::gl::Shader>(GL_GEOMETRY_SHADER, ge::core::loadTextFile(shaderDir+"skybox_gs.glsl")));
    auto skybox_fs(std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, ge::core::loadTextFile(shaderDir+"skybox_fs.glsl")));
    auto program(std::make_shared<ge::gl::Program>(skybox_vs, skybox_gs, skybox_fs));
    if (!program.get()->isProgram()) {
        std::cerr << "Problem with skyboxVT shaders" << std::endl;
    }

    _skyboxVT->gl = _gl;
    _skyboxVT->program = program;
    _skyboxVT->perspectiveCamera = perspectiveCamera;
    _skyboxVT->orbitCamera = orbitCamera;
    _skyboxVT->viewport = _viewPort;


    QtImageLoaderWrapper loader;
    CubeMapTextureFactory::dir = APP_RESOURCES"/texture/skybox/";
    std::shared_ptr<msg::CubeMapTexture> cubeMap = msg::CubeMapTextureFactory::create("right_1.png","left.png","top.png","down.png","center.png","right_2.png", loader);
    cubeMap->gl = _gl;
    cubeMap->initTexture();
    _skyboxVT->cubeMap = cubeMap;

    return true;
}
bool msg::Renderer::initVT() {
    std::cout << "Renderer initVT" << std::endl;
    bool i = true;
    i &= initSimpleVT();
    i &= initSkyboxVT();
    i &= initLaserVT();

    
    return i;
}


void msg::Renderer::drawVT() {
    //std::cout << "Renderer drawVT" << std::endl;
    _skyboxVT->draw();
    _simpleVT->draw();
    _laserVT->draw();
}
