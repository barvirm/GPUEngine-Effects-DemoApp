#pragma once

#include "Collision.h"
#include <memory>
#include <vector>

namespace msg {
    class LaserManager;
    class ShieldManager;
}

namespace msg {
    class LaserShieldCollision : public Collision {
    public:
        LaserShieldCollision(std::shared_ptr<msg::LaserManager> &l, std::shared_ptr<msg::ShieldManager> &s) :
            laserManager(l),
            shieldManager(s)
        {}
        virtual void update() override;
        std::shared_ptr<msg::LaserManager> laserManager;
        std::shared_ptr<msg::ShieldManager> shieldManager;
    };
}