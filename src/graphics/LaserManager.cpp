//
// Created by makku on 11.3.18.
//

#include <LaserManager.h>
#include <geSG/AnimationChannel.h>
#include <sg/AnimationChannel.h>
#include <util/Algorithm.h>
#include <sg/AnimationEventChannel.h>


using namespace std::chrono_literals;

msg::LaserManager::LaserManager(std::shared_ptr<msg::AnimationManager> &animationManager, std::shared_ptr<app::util::Timer<double>> &timer) :
    MissileManager(animationManager, timer) 
{}

void msg::LaserManager::update() {
    //std::cout << ShootingAnimationMap.size() << std::endl;
}

void msg::LaserManager::getShotingAnimation(const std::shared_ptr<msg::Laser> &missile, std::shared_ptr<ge::sg::Animation> &animation) {
    auto anim(std::make_shared<ge::sg::Animation>());
    auto mvch(std::make_shared<ge::sg::MovementAnimationChannel>());
    auto evch(std::make_shared<msg::AnimationEventChannel>());

    mvch->positionKF.emplace_back(ge::core::time_point(0s) , glm::vec3(0));
    mvch->positionKF.emplace_back(ge::core::time_point(20s), missile->direction() * 200.0f);
    mvch->setTarget(missile->matrix());

    auto event = [this, missile] {
        ste::vector_erase(*missiles, missile);
        ShootingAnimationMap.erase( missile.get() );
    };

    auto animationEvent = msg::AnimationEventFactory::create(event);
    evch->KF.emplace_back(ge::core::time_point(20s), animationEvent);

    anim->channels.emplace_back(mvch);
    anim->channels.emplace_back(evch);
    animation = anim;
}

void msg::LaserManager::getFinishAnimation(const std::shared_ptr<msg::Laser> &missile, std::shared_ptr<ge::sg::Animation> &animation) {
    auto anim(std::make_shared<ge::sg::Animation>());
    auto mvch(std::make_shared<msg::AnimationChannel<glm::vec3>>());
    auto evch(std::make_shared<msg::AnimationEventChannel>());

    auto animationDuration = ge::core::time_point(0.5s);

    mvch->KF.emplace_back(ge::core::time_point(0s)  , missile->begin());
    mvch->KF.emplace_back(animationDuration         , missile->end());
    mvch->setTarget(missile->beginPtr());

    auto event = [this, missile] { 
        ste::vector_erase(*missiles, missile);
        ShootingAnimationMap.erase(missile.get());
        FinishAnimationMap.erase(missile.get());
    };

    auto animationEvent = msg::AnimationEventFactory::create(event);
    evch->KF.emplace_back(animationDuration         , animationEvent);

    anim->channels.emplace_back(evch);
    anim->channels.emplace_back(mvch);
    animation = anim;
}

void msg::LaserManager::addLaser(const glm::vec3 &&start, const glm::vec3 &&end, const glm::vec4 &&color, float &&f) {
    auto l(std::make_shared<msg::Laser>(start, end, color, f));
    addMissile(l);
}
void msg::LaserManager::addLaser(const glm::vec3 &start, const glm::vec3 &end, const glm::vec4 &color, float &f) {
    auto l(std::make_shared<msg::Laser>(start, end, color, f));
    addMissile(l);
}
