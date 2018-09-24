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
    for(auto &shield : *shieldManager->shields) {
        for(const auto &laser : *laserManager->missiles) {
            glm::mat4 model = *laser->getMatrix();
            glm::vec3 worldSpaceBegin(model * glm::vec4(laser->getBegin(), 1.0f));
            glm::vec3 worldSpaceEnd(model * glm::vec4(laser->getEnd(), 1.0f));
            const msg::Line l(worldSpaceBegin, worldSpaceEnd);
            // TODO end
            if ( LineSphereIntersector::intersects(l, shield) && !laserManager->FinishAnimationMap[laser.get()] ) {
                auto point(LineSphereIntersector::computeIntersection(*laser, shield));
                laserManager->stopAnimation(laser);
                shieldManager->addIntersection(shield, point);
                shieldManager->addPulseWave(shield, point);
                laserManager->startHitAnimation(laser);
            }
        }
    }
}
