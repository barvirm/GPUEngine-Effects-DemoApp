#pragma once

#include <MissileManager.h>
#include <Effects/laser/Laser.h>

namespace ge {
    namespace util {
        class OrbitCamera;
    }
}

namespace msg { 
    class LaserManager : public MissileManager<msg::Laser> {
        public:
        void addLaser(const glm::vec3 &&start, const glm::vec3 &&end, const glm::vec4 &&color, float &&f);
        void addLaser(const glm::vec3 &start, const glm::vec3 &end, const glm::vec4 &color, float &f);
        void update() override;
        std::shared_ptr<ge::util::OrbitCamera> orbitCamera;
        protected:

        virtual void getShotingAnimation(std::shared_ptr<msg::Laser> &missile, std::shared_ptr<ge::sg::Animation> &animation) override;
        virtual void getFinishAnimation (std::shared_ptr<msg::Laser> &missile, std::shared_ptr<ge::sg::Animation> &animation) override;
    };

}
