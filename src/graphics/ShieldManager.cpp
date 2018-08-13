#include <ShieldManager.h>
#include <Effects/shield/Shield.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

void msg::ShieldManager::addShield(glm::vec3 center, float radius) {
    glm::mat4 m(1.0);
    m *= glm::translate(m, center);
    m *= glm::scale(m, glm::vec3(radius));
    auto shield(std::make_shared<msg::Shield>(center, radius, std::make_shared<glm::mat4>(m)));
    shields->emplace_back(shield);
};

msg::ShieldManager::ShieldManager() {
    std::cout << "ShieldManager ctor" << std::endl;
    shields = std::make_shared<std::vector<std::shared_ptr<msg::Shield>>>();
}

