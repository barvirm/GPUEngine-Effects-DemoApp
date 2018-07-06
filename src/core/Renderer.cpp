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
#include <iostream>
#include <sg/CubeMapTexture.h>

#include <skybox/SkyboxVT.h>

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
    _clock(std::chrono::high_resolution_clock::now())
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
    //_gl->glViewport(0,0,1200,1000);
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
    //std::cout << "Renderer update" << std::endl;

    if (_sceneToProcess) {
        std::cout << "Scene Processing" << std::endl;
        _glScene = ge::glsg::GLSceneProcessor::processScene(_scene,_gl);
        _simpleVT->setScene(_glScene);
        _simpleVT->processScene();
        _sceneToProcess = false;
    }
    _simpleVT->program->setMatrix4fv("projectionMatrix", glm::value_ptr(perspectiveCamera->getProjection()));
    _simpleVT->program->setMatrix4fv("viewMatrix", glm::value_ptr(orbitCamera->getView()));
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

bool msg::Renderer::initVT() {
    std::cout << "Renderer initVT" << std::endl;
    bool i = true;
    i &= initSimpleVT();
    i &= initSkyboxVT();
    
    return i;
}

bool msg::Renderer::initSimpleVT() {
    std::cout << "Renderer initSimpleVT" << std::endl;

    std::string shaderDir(APP_RESOURCES"/shaders/");
    _simpleVT->gl = _gl;

    std::shared_ptr<ge::gl::Shader> simple_vs(std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, ge::core::loadTextFile(shaderDir+"simple_vs.glsl")));
    std::shared_ptr<ge::gl::Shader> simple_fs(std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, ge::core::loadTextFile(shaderDir+"simple_fs.glsl")));
    std::shared_ptr<ge::gl::Program> prog = std::make_shared<ge::gl::Program>(simple_vs, simple_fs);
    if (!prog.get()->isProgram()) {
        std::cerr << "Problem with simpleVT shaders" << std::endl;
    }
    _simpleVT->program = prog;
    return true;
}

bool msg::Renderer::initSkyboxVT() {
    std::cout << "Renderer initSkyboxVT" << std::endl;

    std::string shaderDir(APP_RESOURCES"/shaders/");
    std::shared_ptr<ge::gl::Shader> skybox_vs(std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, ge::core::loadTextFile(shaderDir+"skybox_vs.glsl")));
    std::shared_ptr<ge::gl::Shader> skybox_gs(std::make_shared<ge::gl::Shader>(GL_GEOMETRY_SHADER, ge::core::loadTextFile(shaderDir+"skybox_gs.glsl")));
    std::shared_ptr<ge::gl::Shader> skybox_fs(std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, ge::core::loadTextFile(shaderDir+"skybox_fs.glsl")));
    std::shared_ptr<ge::gl::Program> program = std::make_shared<ge::gl::Program>(skybox_vs, skybox_gs, skybox_fs);
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


void msg::Renderer::drawVT() {
    //std::cout << "Renderer drawVT" << std::endl;
    _skyboxVT->draw();
    _simpleVT->draw();
    
}
