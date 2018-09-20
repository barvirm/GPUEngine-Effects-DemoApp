#pragma once

#include <memory>
#include <glm/fwd.hpp>
#include <vector>
#include <geCore/Updatable.h>


namespace msg {
    class Shield;
    class AnimationManager;
}

namespace msg {
    class ShieldManager {
        public:
            ShieldManager();
            ~ShieldManager() = default;

            void addShield(glm::vec3 center, float radius);
            void addPulseWave(msg::Shield &shield, glm::vec3 origin);
            void addIntersection(msg::Shield &shield, glm::vec3 origin, ge::core::time_point &t);
            
            std::shared_ptr<std::vector<msg::Shield>> shields;
            std::shared_ptr<msg::AnimationManager> animationManager;
            std::shared_ptr<double> time;
    };
}
