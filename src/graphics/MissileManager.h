#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <geSG/Animation.h>
#include <sg/AnimationManager.h>
#include <iostream>

namespace msg {
    class AnimationManager;
}

namespace msg {
    template <class T>
    class MissileManager {
    public:
        MissileManager(std::shared_ptr<AnimationManager> &animManager, std::shared_ptr<double> &t);
        void stopAnimation(const std::shared_ptr<const T> &missile);
        void startHitAnimation(const std::shared_ptr<T> &missile);
        virtual void update() = 0;

        std::shared_ptr<std::vector<std::shared_ptr<T>>> missiles;
        
        std::unordered_map<const T *, std::shared_ptr<ge::sg::Animation>> ShootingAnimationMap;
        std::unordered_map<const T *, std::shared_ptr<ge::sg::Animation>> FinishAnimationMap;
    protected:
        std::shared_ptr<msg::AnimationManager> animationManager;
        std::shared_ptr<double> time;
        void addMissile(std::shared_ptr<T> &missile);
        virtual void getShotingAnimation(const std::shared_ptr<T> &missile, std::shared_ptr<ge::sg::Animation> &animation) = 0;
        virtual void getFinishAnimation (const std::shared_ptr<T> &missile, std::shared_ptr<ge::sg::Animation> &animation) = 0;


    };

    template<class T>
    MissileManager<T>::MissileManager(std::shared_ptr<AnimationManager> &animationManager, std::shared_ptr<double> &time) :
        missiles(std::make_shared<std::vector<std::shared_ptr<T>>>()),
        animationManager(animationManager),
        time(time)
    {}

    template<class T>
    void MissileManager<T>::addMissile(std::shared_ptr<T> &missile) {
        missiles->emplace_back(missile);
        std::shared_ptr<ge::sg::Animation> animation;
        getShotingAnimation(missile,animation);
        ShootingAnimationMap[missile.get()] = animation;
        auto now(std::chrono::duration<double>(*time.get()));
        animationManager->playAnimation(animation, now);
    }

    template<class T>
    void MissileManager<T>::stopAnimation(const std::shared_ptr<const T> &missile) {
        animationManager->removeAnimation(ShootingAnimationMap[missile.get()]);
    }

    template<class T>
    void MissileManager<T>::startHitAnimation(const std::shared_ptr<T> &missile) {
        std::shared_ptr<ge::sg::Animation> animation;
        getFinishAnimation(missile, animation);
        FinishAnimationMap[missile.get()] = animation;
        auto now(std::chrono::duration<double>(*time.get()));
        animationManager->playAnimation(animation, now);
    }
}

