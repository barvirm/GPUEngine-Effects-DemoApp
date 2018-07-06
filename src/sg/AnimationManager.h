#pragma once

#include <geSG/AnimationManager.h>

namespace ge {
    namespace sg {
        class Animation;
    }
}

namespace msg {
    class AnimationManager : public ge::sg::AnimationManager {
    public:
        AnimationManager() : ge::sg::AnimationManager() {};
        void removeAnimation(std::shared_ptr<ge::sg::Animation> &animation);
    };
}
