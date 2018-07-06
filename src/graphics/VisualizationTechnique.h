#pragma once

namespace ge {
    namespace sg {
        class MatrixTransformation;
        class Material;
        class Mesh;
    }
}


namespace msg {

    class VisualizationTechnique {
    public:
        virtual ~VisualizationTechnique() = default;

        virtual void update() = 0;
        virtual void draw() = 0;
    };
}
