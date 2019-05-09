
#include <SimpleVT.h>

#include <glsg/GLScene.h>
#include <glsg/EnumToGL.h>
#include <geGL/geGL.h>
#include <geGL/VertexArray.h>
#include <geGL/Texture.h>
#include <geSG/Scene.h>
#include <geSG/Material.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <geUtil/OrbitCamera.h>
#include <geUtil/PerspectiveCamera.h>

void msg::SimpleVT::setScene(std::shared_ptr<ge::glsg::GLScene> &scene) {
    glScene = scene;
}

void msg::SimpleVT::processScene() {

    for (auto &meshIt : glScene->GLMeshes) {
        std::shared_ptr<glm::mat4> matrix = std::make_shared<glm::mat4>(1);
        matrices.emplace_back(matrix);
        matriciesContainer[meshIt.first] = matrix;
        std::shared_ptr<ge::gl::VertexArray> VAO = std::make_shared<ge::gl::VertexArray>(gl->getFunctionTable());
        for (auto &glAttrib : meshIt.second) {
            if (glAttrib.attributeDescriptor->semantic == ge::sg::AttributeDescriptor::Semantic::indices) {
                VAO->addElementBuffer(glAttrib.BO);
            }
            else {
                int attribLocation = semantic2Attribute(glAttrib.attributeDescriptor->semantic);
                if (attribLocation != -1)
                    VAO->addAttrib(glAttrib.BO,
                                   static_cast<GLuint>(attribLocation),
                                   glAttrib.attributeDescriptor->numComponents,
                                   ge::glsg::translateEnum(glAttrib.attributeDescriptor->type),
                                   static_cast<GLsizei>(glAttrib.attributeDescriptor->stride)
                    );
            }
        }

        VAOContainer[meshIt.first] = VAO;

        ge::sg::Material *mat = meshIt.first->material.get();

        auto component = mat->getComponent<ge::sg::MaterialSimpleComponent>(ge::sg::MaterialSimpleComponent::Semantic::diffuseColor);
        if (component) {
            unsigned sizeInBytes = component->size * component->getSize(component->dataType);
            colorContainer[mat] = std::make_unique<unsigned char[]>(sizeInBytes);
            std::copy_n(component->data.get(), sizeInBytes, colorContainer[mat].get());
        }

        for (auto &comp : mat->materialComponents) {
            if (comp->getType() == ge::sg::MaterialComponent::ComponentType::IMAGE) {
                auto imageComponent = dynamic_cast<ge::sg::MaterialImageComponent *>(comp.get());
                diffuseTextureContainer[meshIt.first] = glScene->textures[imageComponent];
            }
        }
    }
}

void msg::SimpleVT::update() {
    program->setMatrix4fv("projectionMatrix", glm::value_ptr(perspectiveCamera->getProjection()));
    program->setMatrix4fv("viewMatrix", glm::value_ptr(orbitCamera->getView()));
}

void msg::SimpleVT::draw() {
	// std::cout << "SIMPLE DRAW" << std::endl;
    program->use();

    if (!glScene) return;
    for (auto &meshIt : glScene->GLMeshes) {
        ge::sg::Mesh *mesh = meshIt.first;
        std::shared_ptr<glm::mat4> matrix = matriciesContainer[mesh];
        program->set4fv("color", reinterpret_cast<const float *>(colorContainer[mesh->material.get()].get()));
        program->setMatrix4fv("modelMatrix", value_ptr(*matrix));
        ge::gl::Texture *texture = diffuseTextureContainer[mesh].get();
        texture->bind(0);

        ge::gl::VertexArray *VAO = VAOContainer[mesh].get();
        VAO->bind();

        gl->glDrawElements(
                ge::glsg::translateEnum(mesh->primitive),
                (GLsizei) mesh->count,
                ge::glsg::translateEnum(mesh->getAttribute(ge::sg::AttributeDescriptor::Semantic::indices)->type),
                0
        );
        VAO->unbind();
    }
}

int msg::SimpleVT::semantic2Attribute(ge::sg::AttributeDescriptor::Semantic semantic) {
    switch (semantic) {
        case ge::sg::AttributeDescriptor::Semantic::position:
            return 0;
        case ge::sg::AttributeDescriptor::Semantic::texcoord:
            return 2;
        default:
            return -1;
    }
}


