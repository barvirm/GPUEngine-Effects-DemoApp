#include <ShieldManager.h>

#include <Effects/shield/Shield.h>
#include <Effects/shield/Intersection.h>
#include <Effects/shield/PulseWave.h>
#include <geSG/Animation.h>
#include <sg/AnimationChannel.h>
#include <sg/AnimationManager.h>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <memory>



void msg::ShieldManager::addShield(glm::vec3 center, float radius) {
    auto m(std::make_shared<glm::mat4>(
		glm::vec4(radius, 0, 0, 0),
        glm::vec4(0, radius, 0, 0),
        glm::vec4(0, 0, radius, 0),
        glm::vec4(center, 1.0)
	));
    msg::Shield shield(center, radius, m);
    shields->emplace_back(shield);
};

msg::ShieldManager::ShieldManager() {
    std::cout << "ShieldManager ctor" << std::endl;
    shields = std::make_shared<std::vector<msg::Shield>>();
}

void msg::ShieldManager::addPulseWave(msg::Shield &shield, glm::vec3 origin) {
    using namespace std::chrono_literals;
    msg::PulseWave pulseWave(0, origin);

    auto anim(std::make_shared<ge::sg::Animation>());
    auto floatChannel(std::make_shared<msg::AnimationChannel<float>>());
    auto PulseWavetime = pulseWave.getTime();

    floatChannel->setTarget(PulseWavetime);
    anim->channels.push_back(floatChannel);

    floatChannel->KF.emplace_back(0s, 0);
    floatChannel->KF.emplace_back(3s, 1);
    shield.addPulseWave(pulseWave);

    animationManager->playAnimation(anim, ge::core::time_point(std::chrono::duration<double>(*time.get())));

}

void msg::ShieldManager::addIntersection(msg::Shield &shield, glm::vec3 origin) {
    using namespace std::chrono_literals;
    msg::Intersection intersection(origin);

    auto anim(std::make_shared<ge::sg::Animation>());
    auto floatChannel(std::make_shared<msg::AnimationChannel<float>>());
    auto target = intersection.time();
    floatChannel->setTarget(target);
    anim->channels.push_back(floatChannel);

    floatChannel->KF.emplace_back(0s, 0.0f);
    floatChannel->KF.emplace_back(0.5s, 1.0f);

    shield.addIntersection(intersection);
    animationManager->playAnimation(anim, ge::core::time_point(std::chrono::duration<double>(*time.get())));

}

