#include <ShieldManager.h>

#include <Effects/shield/Shield.h>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

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

