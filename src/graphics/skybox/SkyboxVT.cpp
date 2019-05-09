#include <skybox/SkyboxVT.h>
#include <sg/CubeMapTexture.h>

#include <geGL/geGL.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <geUtil/PerspectiveCamera.h>
#include <geUtil/OrbitCamera.h>


void msg::SkyboxVT::draw() {

    if (VAO == nullptr) {
        VAO = std::make_shared<ge::gl::VertexArray>(gl->getFunctionTable());
    }

    glm::vec3 dummy1;
    glm::vec4 dummy2;
    glm::quat orientation;
    glm::mat4 viewMatrix = orbitCamera->getView();
    glm::mat4 projectionMatrix = perspectiveCamera->getProjection();

    glm::decompose(viewMatrix, dummy1, orientation, dummy1, dummy1, dummy2);

    program->use();
    program->setMatrix4fv("InverseProjectionViewMatrix", glm::value_ptr(glm::inverse(projectionMatrix * glm::inverse(glm::mat4_cast(orientation)))));
    program->set("WindowSize", static_cast<unsigned>(viewport->x), static_cast<unsigned>(viewport->y));
    VAO->bind();
    cubeMap->texture->bind(0);
    gl->glDepthMask(GL_FALSE);
    gl->glDrawArrays(GL_POINTS, 0, 1);
    gl->glDepthMask(GL_TRUE);

}
