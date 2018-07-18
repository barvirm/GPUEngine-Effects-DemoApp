//
// Created by makku on 11.3.18.
//

#include <LaserManager.h>
#include <geCore/Updatable.h>
#include <geSG/AnimationChannel.h>
#include <sg/AnimationChannel.h>
#include <geUtil/OrbitCamera.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>


using namespace std::chrono_literals;
void msg::LaserManager::update() {
    glm::vec3 _cameraPosition = static_cast<glm::vec3>( glm::inverse(orbitCamera->getView())[3] );

    auto remove_function = [this, &_cameraPosition](std::shared_ptr<msg::Laser> l) {
        // laser is too far or animation finished  -> destroy laser
        if (glm::distance(_cameraPosition, l->getBegin()) > 500.0f || l->getLength() < 0.01f) {
            ShootingAnimationMap.erase(l.get());
            FinishAnimation.erase(l.get());
            return true;
        }
        return false;
    };

    missiles->erase(std::remove_if(missiles->begin(), missiles->end(), remove_function), missiles->end());
}

void msg::LaserManager::getShotingAnimation(std::shared_ptr<msg::Laser> &missile, std::shared_ptr<ge::sg::Animation> &animation) {
    std::shared_ptr<ge::sg::Animation> anim(std::make_shared<ge::sg::Animation>());
    std::shared_ptr<ge::sg::MovementAnimationChannel> mvch(std::make_shared<ge::sg::MovementAnimationChannel>());
    anim->channels.emplace_back(mvch);

    mvch->positionKF.emplace_back(ge::core::time_point(0s), glm::vec3(0,0,0));
    mvch->positionKF.emplace_back(ge::core::time_point(10s), missile->getDirection() * 100.0f);

    mvch->setTarget(missile->getMatrix());
    animation = anim;
}

void msg::LaserManager::getFinishAnimation(std::shared_ptr<Laser> &missile, std::shared_ptr<ge::sg::Animation> &animation) {
    std::shared_ptr<ge::sg::Animation> anim(std::make_shared<ge::sg::Animation>());
    std::shared_ptr<msg::AnimationChannel<glm::vec3>> mvch = std::make_shared<msg::AnimationChannel<glm::vec3>>();
    mvch->setTarget(missile->getBeginPtr());
    anim->channels.push_back(mvch);

    mvch->KF.emplace_back(ge::core::time_point(0s), missile->getBegin());
    mvch->KF.emplace_back(ge::core::time_point(0.2s), missile->getEnd());

    animation = anim;
}

