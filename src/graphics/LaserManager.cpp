//
// Created by makku on 11.3.18.
//

#include <LaserManager.h>
#include <geCore/Updatable.h>
#include <geSG/AnimationChannel.h>
#include <sg/AnimationChannel.h>
#include <util/Algorithm.h>
#include <geUtil/OrbitCamera.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <sg/AnimationEventChannel.h>


using namespace std::chrono_literals;

void msg::LaserManager::update() {
    glm::vec3 _cameraPosition = static_cast<glm::vec3>( glm::inverse(orbitCamera->getView())[3] );

    auto predicate = [& _cameraPosition](glm::vec3 laserBegin) {
        return glm::distance(_cameraPosition, laserBegin) > 500.0f;
    };

    auto predicate_wrapper_map = [&predicate](auto &pair) {
        return predicate(pair.first->getBegin());
    };

    auto predicate_wrapper_vector = [&predicate](auto &l) {
        return predicate(l->getBegin());
    };
    ste::unordered_map_erase_if(ShootingAnimationMap, predicate_wrapper_map);
    missiles->erase(std::remove_if(missiles->begin(), missiles->end(), predicate_wrapper_vector), missiles->end());
}

void msg::LaserManager::getShotingAnimation(std::shared_ptr<msg::Laser> &missile, std::shared_ptr<ge::sg::Animation> &animation) {
    auto anim(std::make_shared<ge::sg::Animation>());
    auto mvch(std::make_shared<ge::sg::MovementAnimationChannel>());

    mvch->positionKF.emplace_back(ge::core::time_point(0s) , glm::vec3(0));
    mvch->positionKF.emplace_back(ge::core::time_point(10s), missile->getDirection() * 100.0f);
    mvch->setTarget(missile->getMatrix());

    anim->channels.emplace_back(mvch);
    animation = anim;
}

void msg::LaserManager::getFinishAnimation(std::shared_ptr<Laser> &missile, std::shared_ptr<ge::sg::Animation> &animation) {
    auto anim(std::make_shared<ge::sg::Animation>());
    auto mvch(std::make_shared<msg::AnimationChannel<glm::vec3>>());
    auto evch(std::make_shared<msg::AnimationEventChannel>());

    mvch->KF.emplace_back(ge::core::time_point(0s)  , missile->getBegin());
    mvch->KF.emplace_back(ge::core::time_point(0.2s), missile->getEnd());
    mvch->setTarget(missile->getBeginPtr());

    auto event = [this, &missile] { 
        missiles->erase(std::remove(missiles->begin(), missiles->end(), missile)); 
    };
    auto animationEvent = msg::AnimationEventFactory::create(event);
    evch->KF.emplace_back(ge::core::time_point(0.2s), animationEvent);

    anim->channels.push_back(evch);
    anim->channels.push_back(mvch);
    animation = anim;
}

