#pragma once

#include <geSG/AnimationChannel.h>
#include <memory>

namespace msg {
    template<class T>
    class AnimationChannel : public ge::sg::AnimationChannel {
    public:
        using KeyFrame = ge::sg::AnimationKeyFrameTemplate<T>;
        using Interpolator = ge::sg::KeyframeInterpolator<std::vector<KeyFrame>>;
        AnimationChannel();
        ~AnimationChannel() override = default;

        void update(const ge::core::time_point &t) override;
        inline void setTarget(std::shared_ptr<T> &target) { _target = target; }
        inline std::shared_ptr<T> &getTarget() { return _target; }
        inline float getTargetFloat() { return *_target; }

        ge::core::time_unit getDuration() const override;

    public:
        std::vector<KeyFrame> KF;
        std::unique_ptr<Interpolator> interpolator;

    protected:
        std::shared_ptr<T> _target;
    };

    template<class T>
    AnimationChannel<T>::AnimationChannel() : interpolator(new ge::sg::LinearKeyframeInterpolator<std::vector<KeyFrame>>()) {}

    template<class T>
    ge::core::time_unit AnimationChannel<T>::getDuration() const {
        ge::core::time_point end = KF.empty() ? ge::core::time_point() : KF.back().getTime();
        return std::chrono::duration_cast<ge::core::time_unit>(end.time_since_epoch());
    }

    template<class T>
    void AnimationChannel<T>::update(const ge::core::time_point &t) {
        T p = interpolator->interpolate(KF, ge::core::TPtoFP(t));

        if (_target) {
            *_target = p;
        }
    }
}
