#pragma once

#include <GERendererBase.h>

namespace ge {
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
    class Renderer : public GERendererBase {
        Q_OBJECT
    public:
        Renderer(QObject *parent = nullptr);
        void setupGLState();
        std::shared_ptr<ge::util::OrbitCamera> orbitCamera;
        std::shared_ptr<ge::util::PerspectiveCamera> perspectiveCamera;
        virtual void setScene(std::shared_ptr<ge::sg::Scene>& loadedScene);
    public slots:
        virtual void beforeRendering() override;
   protected:
        virtual void update();
        virtual void onViewportChanged() override;
        virtual void onContextCreated() override;

        std::shared_ptr<ge::sg::Scene> _scene;
        std::shared_ptr<ge::glsg::GLScene> _glScene;

        std::shared_ptr<double> _time;
        bool _sceneToProcess;

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
