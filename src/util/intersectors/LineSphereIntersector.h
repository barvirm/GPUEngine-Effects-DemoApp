#pragma once
#include <geSG/RaySphereIntersector.h>

namespace msg {
    class Line;
    class Sphere;
}


namespace msg {
    class LineSphereIntersector : protected ge::sg::RaySphereIntersector {
        public:
            static bool intersects(const msg::Line &line, const msg::Sphere &sphere);
            static glm::vec3 computeIntersection(const msg::Line &line, const msg::Sphere &sphere);
    };
}