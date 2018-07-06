#include <QOpenGLContext>
#include <QuickRendererBase.h>
#include <iostream>


QuickRendererBase::QuickRendererBase(QObject *parent) : 
    QObject(parent),
    _qqw(nullptr)
{
    std::cout << "QuickRendererBase ctor" << std::endl;
    QQuickWindow* qqw = dynamic_cast<QQuickWindow *>(parent);

    if (qqw != nullptr) {
        setParentWindow(qqw);      
    }
}

void QuickRendererBase::onWidthChanged(int w) {
    std::cout << "QuickRendererBase WidthChanged" << std::endl;
}

void QuickRendererBase::onHeightChanged(int h) {
    std::cout << "QuickRendererBase HeightChanged" << std::endl;
}

void QuickRendererBase::beforeRendering() {
    std::cout << "QuickRendererBase beforeRendering" << std::endl;
    if (!_qqw) {
        std::cerr << "Set parent qqw with setParentWindow()" << std::endl;
        return;
    }

    //when you change the opengl state you must call this at the end.
    //_qqw->resetOpenGLState();
}

void QuickRendererBase::onOGLContextCreated(QOpenGLContext * context)
{
    std::cout << "QuickRendererBase onOGLContextCreated" << std::endl;
}

void QuickRendererBase::setParentWindow(QQuickWindow * qqw) {
    std::cout << "QuickRendererBase setParentWindow" << std::endl;
    if (_qqw != nullptr) {
        if (_qqw == qqw) return;
        disconnect(qqw, SIGNAL(beforeRendering()), this, SLOT(beforeRendering()));
        disconnect(qqw, SIGNAL(openglContextCreated(QOpenGLContext *)), this, SLOT(onOGLContextCreated(QOpenGLContext *)));
        disconnect(qqw, SIGNAL(sceneGraphInvalidated()), this, SLOT(onSceneGraphInvalidated()));
        disconnect(qqw, SIGNAL(frameSwapped()), this, SLOT(onFrameSwapped()));
    }
    _qqw = qqw;
    if(qqw != nullptr) {
        connect(qqw, SIGNAL(beforeRendering()), this, SLOT(beforeRendering()), Qt::DirectConnection);
        connect(qqw, SIGNAL(openglContextCreated(QOpenGLContext *)), this, SLOT(onOGLContextCreated(QOpenGLContext *)), Qt::DirectConnection);
        connect(qqw, SIGNAL(sceneGraphInvalidated()), this, SLOT(onSceneGraphInvalidated()), Qt::DirectConnection);
        connect(qqw, SIGNAL(frameSwapped()), this, SLOT(onFrameSwapped()), Qt::DirectConnection);
        connect(qqw, SIGNAL(widthChanged(int)), this, SLOT(onWidthChanged(int)), Qt::DirectConnection);
        connect(qqw, SIGNAL(heightChanged(int)), this, SLOT(onHeightChanged(int)), Qt::DirectConnection);
        _qqw->setClearBeforeRendering(false);
    }
}

void QuickRendererBase::onSceneGraphInvalidated()
{
    std::cout << "QuickRendererBase onSceneGraphInvalidated" << std::endl;
}

/**
 * Reimplement to react on frame swap.
 */
void QuickRendererBase::onFrameSwapped()
{
    //std::cout << "QuickRendererBase onFrameSwapped" << std::endl;
}


