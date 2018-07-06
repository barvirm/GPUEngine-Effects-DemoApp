#pragma once

#include <VisualizationTechnique.h>
#include <memory>
#include <glm/fwd.hpp>

namespace ge {
    namespace gl {
        class Program;
        class VertexArray;
        class Context;
    }
    namespace util {
        class OrbitCamera;
        class PerspectiveCamera;
    }
}

namespace msg {
    class CubeMapTexture;
}

namespace msg {
    class SkyboxVT : public VisualizationTechnique {
    public:
        std::shared_ptr<msg::CubeMapTexture> cubeMap;
        std::shared_ptr<ge::gl::Program> program;
        std::shared_ptr<ge::util::OrbitCamera> orbitCamera;
        std::shared_ptr<ge::util::PerspectiveCamera> perspectiveCamera;
        std::shared_ptr<ge::gl::Context> gl;
        std::shared_ptr<ge::gl::VertexArray> VAO;
        std::shared_ptr<glm::vec2> viewport;

        void draw() override;
        void update() override {};
    };
}
