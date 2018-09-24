#pragma once

#include "Collider.h"
#include <memory>
#include <vector>

namespace msg {
    class LaserManager;
    class ShieldManager;
}

namespace msg {
    class LaserShieldCollider : public msg::Collider {
    public:
        LaserShieldCollider(std::shared_ptr<msg::LaserManager> &l, std::shared_ptr<msg::ShieldManager> &s) :
            laserManager(l),
            shieldManager(s)
        {}
        virtual void update() override;
        std::shared_ptr<msg::LaserManager> laserManager;
        std::shared_ptr<msg::ShieldManager> shieldManager;
    };
}