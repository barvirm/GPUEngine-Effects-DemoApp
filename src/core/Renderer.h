#pragma once

#include <GERendererBase.h>
#include <util/Stopwatch.h>
#include <util/collision/Collider.h>

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
    class ShieldVT;
    class LaserVT;
    class AnimationManager;
    class LaserManager;
    class ShieldManager;
}

namespace msg {
    class Renderer : public GERendererBase {
        Q_OBJECT

    public:
        Renderer(QObject *parent = nullptr);
        void setupGLState();
        std::shared_ptr<ge::util::OrbitCamera> orbitCamera;
        std::shared_ptr<ge::util::PerspectiveCamera> perspectiveCamera;
        std::shared_ptr<app::util::Stopwatch<double>> _stopwatch;
        virtual void setScene(std::shared_ptr<ge::sg::Scene> &loadedScene);

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
        std::shared_ptr<msg::ShieldVT> _shieldVT;
        std::shared_ptr<msg::LaserVT> _laserVT;
        std::shared_ptr<msg::AnimationManager> _animationManager;
        std::shared_ptr<msg::LaserManager> _laserManager;
        std::shared_ptr<msg::ShieldManager> _shieldManager;
        std::vector<std::unique_ptr<msg::Collider>> _colliders;

    private:
        void setupCamera();
        bool initVT();
        bool initSimpleVT();
        bool initSkyboxVT();
        bool initLaserVT();
        bool initTestVT();
        bool initShieldVT();
        void drawVT();
        void initColliders();
    };
}
