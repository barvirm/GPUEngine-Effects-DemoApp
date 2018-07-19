#pragma once
#include <QuickRendererBase.h>
#include <memory>
#include <glm/fwd.hpp>

namespace ge {
    namespace gl {
        class Context;
    }
    namespace Util {
        class OrbitCamera;
        class PerspectiveCamera;
    }
}

namespace msg {
    class GERendererBase : public QuickRendererBase {
        Q_OBJECT
    public:
        GERendererBase(QObject *parent = nullptr);
        void setupGLState();

    public slots:
        virtual void beforeRendering() override;
        virtual void onOGLContextCreated(QOpenGLContext *context) override;
        virtual void onWidthChanged(int w) override;
        virtual void onHeightChanged(int h) override;

    protected:
        virtual void onViewportChanged();
        virtual void onContextCreated();
        std::shared_ptr<ge::gl::Context> _gl;
        std::shared_ptr<glm::vec2> _viewport;
    };
}
