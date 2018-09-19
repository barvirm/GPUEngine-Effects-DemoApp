#include <LineSphereIntersector.h>

#include <Effects/geometry/Line.h>
#include <Effects/geometry/Sphere.h>

bool msg::LineSphereIntersector::intersects(const msg::Line &line, const msg::Sphere &sphere) {
    ge::util::Ray ray;
    ray.direction = line.getDirection();
    ray.origin = line.getBegin();
    float t = ge::sg::RaySphereIntersector::intersects(ray, sphere);
    return (t > 0 && t < line.getLength());
}

glm::vec3 msg::LineSphereIntersector::computeIntersection(const msg::Line &line, const msg::Sphere &sphere) {
    ge::util::Ray ray{};
    ray.direction = line.getDirection();
    ray.origin = line.getBegin();
    float t = ge::sg::RaySphereIntersector::computeIntersection(ray, sphere);
    return line.getBegin() + line.getDirection() * t;
}