#include "LaserShieldCollider.h"
#include <ShieldManager.h>
#include <LaserManager.h>
#include <Effects/geometry/Sphere.h>
#include <Effects/shield/Shield.h>
#include <Effects/shield/Intersection.h>
#include <Effects/shield/PulseWave.h>
#include <intersectors/LineSphereIntersector.h>
#include <glm/gtx/string_cast.hpp>

void msg::LaserShieldCollider::update() {
    using namespace std::chrono_literals;
    for(auto &shield : *shieldManager->shields) {
        for(const auto &laser : *laserManager->missiles) {
            glm::mat4 model = *laser->getMatrix();
            glm::vec3 worldSpaceBegin(model * glm::vec4(laser->getBegin(), 1.0f));
            glm::vec3 worldSpaceEnd(model * glm::vec4(laser->getEnd(), 1.0f));
            if ( LineSphereIntersector::intersects({worldSpaceBegin, worldSpaceEnd}, shield) && !laserManager->FinishAnimationMap[laser.get()] ) {
                auto point(LineSphereIntersector::computeIntersection(*laser, shield));
                laserManager->stopAnimation(laser);
                shieldManager->addIntersection(shield, point,ge::core::time_point(3s));
                shieldManager->addPulseWave(shield, point, ge::core::time_point(3s));
                laserManager->startHitAnimation(laser);
            }
        }
    }
}
