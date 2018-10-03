#pragma once

#include <MissileManager.h>
#include <Effects/laser/Laser.h>


namespace msg { 
    class LaserManager : public MissileManager<msg::Laser> {
        public:
            LaserManager(std::shared_ptr<msg::AnimationManager> &animationManager, std::shared_ptr<double> &time) : MissileManager(animationManager, time) {};
            void addLaser(const glm::vec3 &&start, const glm::vec3 &&end, const glm::vec4 &&color, float &&f);
            void addLaser(const glm::vec3 &start, const glm::vec3 &end, const glm::vec4 &color, float &f);
            void update() override;
        protected:
            virtual void getShotingAnimation(const std::shared_ptr<msg::Laser> &missile, std::shared_ptr<ge::sg::Animation> &animation) override;
            virtual void getFinishAnimation (const std::shared_ptr<msg::Laser> &missile, std::shared_ptr<ge::sg::Animation> &animation) override;
    };

}
