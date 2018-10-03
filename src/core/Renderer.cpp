#include <Renderer.h>

#include <iostream>
#include <util/Algorithm.h>

#include <sg/AnimationManager.h>
#include <sg/CubeMapTexture.h>
#include <sg/AnimationManager.h>
#include <glsg/TextureFactory.h>
#include <glsg/GLSceneProcessor.h>

#include <geGL/geGL.h>
#include <geSG/Material.h>
#include <geCore/Text.h>
#include <geUtil/OrbitCamera.h>
#include <geUtil/PerspectiveCamera.h>

#include <QtImageLoader.h>
#include <SimpleVT.h>
#include <LaserManager.h>
#include <skybox/SkyboxVT.h>
#include <Effects/laser/LaserVT.h>
#include <Effects/shield/ShieldVT.h>
#include <ShieldManager.h>

#include <util/collision/LaserShieldCollider.h>

msg::Renderer::Renderer(QObject *parent) :
    GERendererBase(parent),
    orbitCamera(std::make_shared<ge::util::OrbitCamera>()),
    perspectiveCamera(std::make_shared<ge::util::PerspectiveCamera>()),
    _scene(nullptr),
    _glScene(nullptr),
    _time(std::make_shared<double>()),
    _sceneToProcess(false),
    _animationManager(std::make_shared<msg::AnimationManager>()),
    _laserManager(std::make_shared<msg::LaserManager>(_animationManager, _time)),
    _shieldManager(std::make_shared<msg::ShieldManager>(_animationManager, _time)),
    _stopwatch(std::make_shared<app::util::Stopwatch<double>>())
{
    std::cout << "Renderer ctor" << std::endl;
    _stopwatch->start();
    setupCamera();
}

void msg::Renderer::onViewportChanged() {
    std::cout << "onViewportChanged" << std::endl;
    _gl->glViewport(0, 0, _viewport->x, _viewport->y);
    perspectiveCamera->setAspect(_viewport->x / _viewport->y);
}

void msg::Renderer::onContextCreated() {
    initVT();
    initColliders();
}

void msg::Renderer::initColliders() {
    std::cout << "Init Colliders" << std::endl;
    _colliders.emplace_back(std::make_unique<msg::LaserShieldCollider>(_laserManager, _shieldManager));
}

void msg::Renderer::setupGLState() {
    //std::cout << "Renderer setupGLState" << std::endl;
    _gl->glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    _gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _gl->glEnable(GL_DEPTH_TEST);
}

void msg::Renderer::beforeRendering() {
    //std::cout << "Renderer beforeRendering" << std::endl;
    setupGLState();
    update();
    drawVT();
    _qqw->resetOpenGLState();
    _qqw->update();
}

void msg::Renderer::setScene(std::shared_ptr<ge::sg::Scene> &loadedScene) {
    std::cout << "Renderer setScene" << std::endl;
    if ( loadedScene == _scene) return;
    _scene = loadedScene;
    _sceneToProcess = true;
}

void msg::Renderer::update() {

    *_time = _stopwatch->getTime();
    _animationManager->update(std::chrono::duration<double>(*_time));
    
    if (_sceneToProcess) {
        std::cout << "Scene Processing" << std::endl;
        _sceneToProcess = false;
        _shieldManager->addShield(glm::vec3(0), 3.7f);
        _laserManager->addLaser({0, 0, 8 }, {0, 0, 15}, {0, 0, 1, 1}, 2);
        _laserManager->addLaser({1, 1, 25}, {1, 1, 18}, {0, 0, 1, 1}, 2);
    }
    _laserManager->update();
    stdr::for_each(_colliders, [](auto &c) {c->update();});
    stdr::for_each(_visualizationTechniques, [](auto &vt) {vt->update();});
}

void msg::Renderer::setupCamera() {
    std::cout << "Renderer setupCamera" << std::endl;
    orbitCamera->setDistance(20.f);
    orbitCamera->setYAngle(3.1415f/1.2f);
    orbitCamera->setXAngle(3.1415f/6.5f);
    orbitCamera->setFocus(glm::vec3(0));

    perspectiveCamera->setNear(0.1f);
    perspectiveCamera->setFar(5000.0f);
    perspectiveCamera->setFovy(45.0f);
    perspectiveCamera->setAspect(1000.0f / 800.0f);
}

bool msg::Renderer::initSimpleVT() {
    std::cout << "Renderer initSimpleVT" << std::endl;

    std::string shaderDir(APP_RESOURCES"/shaders/");
    auto _simpleVT(std::make_shared<msg::SimpleVT>());
    _simpleVT->gl = _gl;
    _simpleVT->perspectiveCamera = perspectiveCamera;
    _simpleVT->orbitCamera = orbitCamera;

    auto simple_vs(std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, ge::core::loadTextFile(shaderDir+"simple_vs.glsl")));
    auto simple_fs(std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, ge::core::loadTextFile(shaderDir+"simple_fs.glsl")));
    auto prog(std::make_shared<ge::gl::Program>(simple_vs, simple_fs));

    _simpleVT->program = prog;

    _glScene = ge::glsg::GLSceneProcessor::processScene(_scene,_gl);
    _simpleVT->setScene(_glScene);
    _simpleVT->processScene();

    _visualizationTechniques.emplace_back(_simpleVT);

    return true;
}

bool msg::Renderer::initLaserVT() {
    std::cout << "Renderer initLaserVT" << std::endl;

    std::string shaderDir(APP_RESOURCES"/shaders/");
    auto laser_vs(std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, ge::core::loadTextFile(shaderDir+"laser_vs.glsl")));
    auto laser_fs(std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, ge::core::loadTextFile(shaderDir+"laser_fs.glsl")));
    auto laser_gs(std::make_shared<ge::gl::Shader>(GL_GEOMETRY_SHADER, ge::core::loadTextFile(shaderDir+"laser_gs.glsl")));
    auto program(std::make_shared<ge::gl::Program>(laser_vs, laser_fs, laser_gs));
  
    auto _laserVT(std::make_shared<msg::LaserVT>());
    _laserVT->gl = _gl;
    _laserVT->program = program;
    _laserVT->orbitCamera = orbitCamera;
    _laserVT->lasers = _laserManager->missiles;
    _laserVT->viewport = _viewport;
    _laserVT->perspectiveCamera = perspectiveCamera;

    std::string imagePath(APP_RESOURCES"/texture/laserbolt.png");
    std::shared_ptr<QtImage> image(QtImageLoader::loadImage(imagePath.c_str()));

    auto materialComponent(std::make_shared<ge::sg::MaterialImageComponent>());
    materialComponent->semantic = ge::sg::MaterialImageComponent::Semantic::diffuseTexture;
    materialComponent->filePath = imagePath;
    materialComponent->image = image;
    materialComponent->setType(ge::sg::MaterialComponent::ComponentType::IMAGE);

    std::shared_ptr<ge::glsg::TextureFactory> textureFactory(std::make_shared<ge::glsg::DefaultTextureFactory>());
    
    _laserVT->texture = textureFactory->create(materialComponent.get(), _gl);
    _visualizationTechniques.emplace_back(_laserVT);
    return true;
}

bool msg::Renderer::initShieldVT() {
    std::cout << "Renderer initShieldVT" << std::endl;
    std::string shaderDir(APP_RESOURCES"/shaders/");

    auto shield_vs(std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, ge::core::loadTextFile(shaderDir+"shield_vs.glsl")));
    auto shield_fs(std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, ge::core::loadTextFile(shaderDir+"shield_fs.glsl")));
    auto program(std::make_shared<ge::gl::Program>(shield_vs, shield_fs));

    auto _shieldVT(std::make_shared<msg::ShieldVT>());

    _shieldVT->gl = _gl;
    _shieldVT->program = program;
    _shieldVT->shields = _shieldManager->shields;
    _shieldVT->perspectiveCamera = perspectiveCamera;
    _shieldVT->orbitCamera = orbitCamera;
    _shieldVT->time = _time;
    _shieldVT->init();
    _visualizationTechniques.emplace_back(_shieldVT);

    return true;
}

bool msg::Renderer::initSkyboxVT() {
    std::cout << "Renderer initSkyboxVT" << std::endl;

    std::string shaderDir(APP_RESOURCES"/shaders/");
    auto skybox_vs(std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, ge::core::loadTextFile(shaderDir+"skybox_vs.glsl")));
    auto skybox_gs(std::make_shared<ge::gl::Shader>(GL_GEOMETRY_SHADER, ge::core::loadTextFile(shaderDir+"skybox_gs.glsl")));
    auto skybox_fs(std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, ge::core::loadTextFile(shaderDir+"skybox_fs.glsl")));
    auto program(std::make_shared<ge::gl::Program>(skybox_vs, skybox_gs, skybox_fs));

    auto _skyboxVT(std::make_shared<msg::SkyboxVT>());
    _skyboxVT->gl = _gl;
    _skyboxVT->program = program;
    _skyboxVT->perspectiveCamera = perspectiveCamera;
    _skyboxVT->orbitCamera = orbitCamera;
    _skyboxVT->viewport = _viewport;

    QtImageLoaderWrapper loader;
    CubeMapTextureFactory::dir = APP_RESOURCES"/texture/skybox/";
    auto cubeMap = msg::CubeMapTextureFactory::create("right_1.png","left.png","top.png","down.png","center.png","right_2.png", loader);
    cubeMap->gl = _gl;
    cubeMap->initTexture();
    _skyboxVT->cubeMap = cubeMap;

    _visualizationTechniques.emplace_back(_skyboxVT);
    return true;
}

bool msg::Renderer::initVT() {
    std::cout << "Renderer initVT" << std::endl;
    bool i = true;
    i &= initSkyboxVT();
    i &= initSimpleVT();
    i &= initLaserVT();
    i &= initShieldVT();

    return i;
}

void msg::Renderer::drawVT() {
    //std::cout << "Renderer drawVT" << std::endl;
    stdr::for_each(_visualizationTechniques, [](auto &vt){ vt->draw(); });
}
