#pragma once

#include <Effects/VisualizationTechnique.h>
#include <memory>
#include <glm/fwd.hpp>
#include <vector>


namespace ge::gl {
    class Program;
    class VertexArray;
    class Context;
    class Buffer;
}

namespace ge::util {
    class OrbitCamera;
    class PerspectiveCamera;
}

namespace msg {
    class TestingVT : public VisualizationTechnique {
        public:
            TestingVT(std::shared_ptr<ge::gl::Context> &gl);
            std::shared_ptr<ge::gl::Program> program;
            std::shared_ptr<ge::util::OrbitCamera> orbitCamera;
            std::shared_ptr<ge::util::PerspectiveCamera> perspectiveCamera;
            std::shared_ptr<ge::gl::Context> gl;

            void draw() override;
            void update() override;
        protected:
            std::shared_ptr<ge::gl::VertexArray> VAO;
            std::shared_ptr<ge::gl::Buffer> verticies;
            std::shared_ptr<ge::gl::Buffer> indicies;
    };
}