#pragma once

#include <QObject>
#include <QQuickWindow>

class QuickRendererBase : public QObject {
    Q_OBJECT
    public:
        explicit QuickRendererBase(QObject *parent = nullptr);
        void setParentWindow(QQuickWindow *qqw);
        inline QQuickWindow *getParentWindow() { return _qqw; }
    public slots:
        virtual void beforeRendering();
        virtual void onOGLContextCreated(QOpenGLContext * context);
        virtual void onSceneGraphInvalidated();
        virtual void onFrameSwapped();
        virtual void onWidthChanged(int w);
        virtual void onHeightChanged(int h);
    protected:
        QQuickWindow *_qqw;
};
