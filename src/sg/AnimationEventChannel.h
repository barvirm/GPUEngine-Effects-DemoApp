#pragma once 

#include <geSG/AnimationChannel.h>
#include <sg/AnimationEvent.h>
#include <functional>
#include <memory>

namespace msg {
    class AnimationEventChannel : public ge::sg::AnimationChannel {
        public:
            using KeyFrame = ge::sg::AnimationKeyFrameTemplate<std::shared_ptr<AnimationCallback>>;

            void update(const ge::core::time_point &t) override;
            ge::core::time_unit getDuration() const override;

        public:
            std::vector<KeyFrame> KF;
    };

}
