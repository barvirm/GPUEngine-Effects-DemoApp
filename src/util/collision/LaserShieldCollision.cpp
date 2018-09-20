#include "LaserShieldCollision.h"
#include <ShieldManager.h>
#include <LaserManager.h>
#include <Effects/geometry/Sphere.h>
#include <Effects/shield/Shield.h>
#include <Effects/shield/Intersection.h>
#include <Effects/shield/PulseWave.h>
#include <intersectors/LineSphereIntersector.h>

void msg::LaserShieldCollision::update() {
    using namespace std::chrono_literals;
    for(auto &shield : *shieldManager->shields) {
        for(const auto &laser : *laserManager->missiles) {
            if ( LineSphereIntersector::intersects(*laser, shield) && laserManager->FinishAnimationMap[laser.get()] ) {
                auto point(LineSphereIntersector::computeIntersection(*laser, shield));
                ge::core::time_point animDuration(0.5s);
                
                laserManager->stopAnimation(laser);
                laserManager->startHitAnimation(laser);
                shieldManager->addIntersection(shield, point, animDuration);
                shieldManager->addPulseWave(shield, point);
            }
        }
    }
}
