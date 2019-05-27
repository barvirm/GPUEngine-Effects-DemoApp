#include <testing/TestingVT.h>

#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <geGL/geGL.h>
#include <iostream>
#include <cmath>

#include <geUtil/OrbitCamera.h>
#include <geUtil/PerspectiveCamera.h>

msg::TestingVT::TestingVT(std::shared_ptr<ge::gl::Context> &gl)
 : gl(gl)
{
    std::cout << "ctor testing" << std::endl;

    std::cout << "Create Sphere" << std::endl;
    float t = (1.0f + std::sqrt(5.0f)) / 2.0f;

    std::vector<glm::vec3> vert{
        {-1,  t,  0},
        { 1,  t,  0},
        {-1, -t,  0},
        { 1, -t,  0},
        { 0, -1,  t},
        { 0,  1,  t},
        { 0, -1, -t},
        { 0,  1, -t},
        { t,  0, -1},
        { t,  0,  1},
        {-t,  0, -1},
        {-t,  0,  1}
    };



    verticies = std::make_shared<ge::gl::Buffer>(
        gl->getFunctionTable(),
        sizeof(glm::vec3) * vert.size(), 
        vert.data()
    );
    VAO = std::make_shared<ge::gl::VertexArray>(gl->getFunctionTable());
    VAO->addAttrib(verticies, 0, 3, GL_FLOAT);


    std::vector<unsigned int> indic {
        0, 11, 5,
        0, 5, 1,
        0, 1, 7,
        0, 7, 10,
        0, 10, 11,
        1, 5, 9,
        5, 11, 4,
        11, 10, 2,
        10, 7, 6,
        7, 1, 8,
        3, 9, 4,
        3, 4, 2,
        3, 2, 6,
        3, 6, 8,
        3, 8, 9,
        4, 9, 5,
        2, 4, 11,
        6, 2, 10,
        8, 6, 7,
        9, 8, 1
    };

    // std::copy(indic.begin(), indic.end(), ind.begin());


    indicies = std::make_shared<ge::gl::Buffer>(
        gl->getFunctionTable(),
        sizeof(unsigned int) * indic.size(), 
        indic.data()
    );

}


void msg::TestingVT::draw() {
    // std::cout << "BAK" << std::endl;
    program->use();
    VAO->bind();
    indicies->bind(GL_ELEMENT_ARRAY_BUFFER);

    // std::cout << "pred draw" << std::endl;
    gl->glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    gl->glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);
    gl->glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    // std::cout << "za draw" << std::endl;
}

void msg::TestingVT::update() {
    const glm::mat4 modelMatrix(1);
    program->setMatrix4fv("projectionMatrix", glm::value_ptr(perspectiveCamera->getProjection()));
    program->setMatrix4fv("viewMatrix", glm::value_ptr(orbitCamera->getView()));
    program->setMatrix4fv("modelMatrix", glm::value_ptr(modelMatrix));
}