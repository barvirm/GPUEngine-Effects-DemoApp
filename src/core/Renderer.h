#pragma once

#include <QuickRendererBase.h>
#include <memory>
#include <glm/vec2.hpp>

namespace ge {
    namespace gl {
        class Context;
    }
    namespace util {
        class OrbitCamera;
        class PerspectiveCamera;
    }
    namespace sg {
        class Scene;
    }
    namespace glsg {
        class GLScene;
    }
}

namespace msg {
    class SimpleVT;
    class SkyboxVT;
    class LaserVT;
    class AnimationManager;
    class LaserManager;
}

namespace msg {
    class Renderer : public QuickRendererBase {
        Q_OBJECT
    public:
        Renderer(QObject *parent = nullptr);
        void setupGLState();
        void viewPortResize(int w, int h);
        inline void updateViewPort(int w, int h) { _viewPort->x = w; _viewPort->y = h; };
        std::shared_ptr<ge::util::OrbitCamera> orbitCamera;
        std::shared_ptr<ge::util::PerspectiveCamera> perspectiveCamera;
    public slots:
        virtual void beforeRendering() override;
        virtual void onOGLContextCreated(QOpenGLContext * context) override;
        virtual void setScene(std::shared_ptr<ge::sg::Scene>& loadedScene);
        virtual void onWidthChanged(int w) override;
        virtual void onHeightChanged(int h) override;
   protected:
        virtual void update();
        virtual void onViewportChange();

        std::shared_ptr<ge::gl::Context> _gl;
        std::shared_ptr<ge::sg::Scene> _scene;
        std::shared_ptr<ge::glsg::GLScene> _glScene;

        std::shared_ptr<double> _time;
        bool _sceneToProcess;
        std::shared_ptr<glm::vec2> _viewPort;

        std::shared_ptr<msg::SimpleVT> _simpleVT;
        std::shared_ptr<msg::SkyboxVT> _skyboxVT;
        std::shared_ptr<msg::LaserVT> _laserVT;
        std::shared_ptr<msg::AnimationManager> _animationManager;
        std::chrono::time_point<std::chrono::high_resolution_clock> _clock;
        std::shared_ptr<msg::LaserManager> _laserManager;

    private:
        void setupCamera();
        bool initVT();
        bool initSimpleVT();
        bool initSkyboxVT();
        bool initLaserVT();
        bool initTestVT();
        void drawVT();

    };
}
