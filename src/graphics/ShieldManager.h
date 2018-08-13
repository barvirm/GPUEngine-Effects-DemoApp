#pragma once

#include <memory>
#include <glm/vec3.hpp> // TODO FWD
#include <glm/mat4x4.hpp> // TODO FWD
#include <vector>


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
            
            std::shared_ptr<std::vector<std::shared_ptr<msg::Shield>>> shields;
            std::shared_ptr<msg::AnimationManager> animationManager;
            std::shared_ptr<double> time;
    };
}
