#pragma once

#include <memory>
#include <glm/fwd.hpp>
#include <vector>
#include <geCore/Updatable.h>


namespace msg {
    class Shield;
    class AnimationManager;
}

namespace app::util {
    template<typename T>
    class Timer;
}

namespace msg {
    class ShieldManager {
        public:
            ShieldManager(std::shared_ptr<msg::AnimationManager> &animationManager, std::shared_ptr<app::util::Timer<double>> &time);
            ~ShieldManager() = default;

            void addShield(glm::vec3 center, float radius);
            void addPulseWave(msg::Shield &shield, glm::vec3 origin, const ge::core::time_point &t);
            void addIntersection(msg::Shield &shield, glm::vec3 origin, const ge::core::time_point &t);
            
            std::shared_ptr<std::vector<msg::Shield>> shields;
        protected:
            std::shared_ptr<msg::AnimationManager> animationManager;
            std::shared_ptr<app::util::Timer<double>> timer;
    };
}
