#pragma once

#include <Effects/VisualizationTechnique.h>
#include <geSG/AttributeDescriptor.h>

#include <glm/mat4x4.hpp>
#include <memory>
#include <unordered_map>

namespace ge {
    namespace gl {
        class Texture;
        class Context;
        class VertexArray;
        class Program;
    }

    namespace sg {
        class Scene;
		class Mesh;
		class Material;
    }

    namespace glsg {
        class GLScene;
    }

    namespace util {
        class OrbitCamera;
        class PerspectiveCamera;
    }
}

namespace msg {
    class SimpleVT : public VisualizationTechnique {
    public:
        virtual void setScene(std::shared_ptr<ge::glsg::GLScene> &scene);
        virtual void processScene();

        void update() override;
        void draw() override;

        inline std::shared_ptr<glm::mat4> &getShipMatrix(int i) { return matrices[i]; }

        static int semantic2Attribute(ge::sg::AttributeDescriptor::Semantic semantic);

        std::shared_ptr<ge::gl::Context> gl;
        std::shared_ptr<ge::gl::Program> program;
        std::shared_ptr<ge::util::OrbitCamera> orbitCamera;
        std::shared_ptr<ge::util::PerspectiveCamera> perspectiveCamera;
        std::vector<std::shared_ptr<glm::mat4>> matrices;

    protected:
        std::shared_ptr<ge::glsg::GLScene> glScene;
        std::unordered_map<ge::sg::Mesh *, std::shared_ptr<ge::gl::VertexArray>> VAOContainer;
        std::unordered_map<ge::sg::Material *, std::unique_ptr<unsigned char[]>> colorContainer;
        std::unordered_map<ge::sg::Mesh *, std::shared_ptr<ge::gl::Texture>> diffuseTextureContainer;
        std::unordered_map<ge::sg::Mesh *, std::shared_ptr<glm::mat4>> matriciesContainer;
    };
}



