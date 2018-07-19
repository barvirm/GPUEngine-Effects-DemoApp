#include "GERendererBase.h"
#include <iostream>
#include <glm/vec2.hpp>
#include <QOpenGLContext>
#include <geGL/geGL.h>
#include <QQuickItem>

msg::GERendererBase::GERendererBase(QObject *parent) :
    QuickRendererBase(parent),
    _gl(nullptr),
    _viewport(std::make_shared<glm::vec2>(1000,800))
{
    std::cout << "GERendererBase ctor" << std::endl;
}

void msg::GERendererBase::onWidthChanged(int w) {
    std::cout << "GERendererBase onWidthChanged" << std::endl;
    _viewport->x = w;
    onViewportChanged();
}

void msg::GERendererBase::onHeightChanged(int h) {
    std::cout << "GERendererBase onHeightChanged" << std::endl;
    _viewport->y = h;
    onViewportChanged();
}

void msg::GERendererBase::onViewportChanged() {
    std::cout << "GERendererBase onViewportChanged" << std::endl;
}

void msg::GERendererBase::onOGLContextCreated(QOpenGLContext *context) {
    std::cout << "GERendererBase onOGLContextCreated" << std::endl;
    context->makeCurrent(_qqw);

    ge::gl::init();
    _gl = std::make_shared<ge::gl::Context>();
    
    onContextCreated();
    context->doneCurrent();
}

void msg::GERendererBase::onContextCreated() {
    std::cout << "GERendererBase onContextCreated" << std::endl;
}

void msg::GERendererBase::beforeRendering() {
    std::cout << "GERendererBase beforeRendering" << std::endl;
}

void msg::GERendererBase::setupGLState() {
    std::cout << "GERendererBase setupGLState" << std::endl;
}
