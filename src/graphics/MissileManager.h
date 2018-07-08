#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <geSG/Animation.h>
#include <sg/AnimationManager.h>

namespace msg {
    class AnimationManager;
}

namespace msg {
    template <class T>
    class MissileManager {
    public:
        MissileManager();
        void addMissile(std::shared_ptr<T> &missile);
        void stopAnimation(std::shared_ptr<T> &missile);
        void startHitAnimation(std::shared_ptr<T> &missile);
        virtual void update() = 0;

        std::shared_ptr<msg::AnimationManager> animationManager;
        std::shared_ptr<std::vector<std::shared_ptr<T>>> missiles;
        std::shared_ptr<double> time;

        std::unordered_map<T *, std::shared_ptr<ge::sg::Animation>> ShootingAnimationMap;
        std::unordered_map<T *, std::shared_ptr<ge::sg::Animation>> FinishAnimation;
    protected:
        virtual void getShotingAnimation(std::shared_ptr<T> &missile, std::shared_ptr<ge::sg::Animation> &animation) = 0;
        virtual void getFinishAnimation (std::shared_ptr<T> &missile, std::shared_ptr<ge::sg::Animation> &animation) = 0;


    };

    template<class T>
    MissileManager<T>::MissileManager() {
        missiles = std::make_shared<std::vector<std::shared_ptr<T>>>();
    }

    template<class T>
    void MissileManager<T>::addMissile(std::shared_ptr<T> &missile) {
        missiles->emplace_back(missile);
        std::shared_ptr<ge::sg::Animation> animation;
        getShotingAnimation(missile,animation);
        ShootingAnimationMap[missile.get()] = animation;
        //animationManager->playAnimation(animation, ge::core::time_point(std::chrono::duration<double>(*time.get())));
    }

    template<class T>
    void MissileManager<T>::stopAnimation(std::shared_ptr<T> &missile) {
        animationManager->removeAnimation(ShootingAnimationMap[missile.get()]);
    }

    template<class T>
    void MissileManager<T>::startHitAnimation(std::shared_ptr<T> &missile) {
        std::shared_ptr<ge::sg::Animation> animation;
        getFinishAnimation(missile, animation);
        FinishAnimation[missile.get()] = animation;
        animationManager->playAnimation(animation, ge::core::time_point(std::chrono::duration<double>(*time.get())));
    }
}

