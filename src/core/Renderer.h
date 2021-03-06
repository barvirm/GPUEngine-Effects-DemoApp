#pragma once

#include <GERendererBase.h>
#include <util/collision/Collider.h>
#include <Effects/VisualizationTechnique.h>

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

namespace app::util {
    template<typename T>
    class Timer;
}

namespace msg {
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
        std::shared_ptr<app::util::Timer<double>> _timer;
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

        std::shared_ptr<msg::AnimationManager> _animationManager;
        std::shared_ptr<msg::LaserManager> _laserManager;
        std::shared_ptr<msg::ShieldManager> _shieldManager;
        std::vector<std::unique_ptr<msg::Collider>> _colliders;
        std::vector<std::shared_ptr<msg::VisualizationTechnique>> _visualizationTechniques;


    private:
        void setupCamera();
        bool initVT();
        bool initSimpleVT();
        bool initSkyboxVT();
        bool initLaserVT();
        bool initShieldVT();
        bool initTestingVT();
        void drawVT();
        void initColliders();


    };
}
