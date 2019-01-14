#pragma once

#include <glm/fwd.hpp>

namespace msg {
    class Transform { 
        public:
            Transform();

            // gettors
            const glm::vec3 orientation() const;
            const glm::vec3 scale() const;
            const glm::vec3 translation() const;
            const glm::mat4 matrix() const;
            const glm::mat4 inverseMatrix() const;

            // settors
            void orientation(const glm::quat &orientation);
            void scale(const glm::vec3 &scale);
            void translation(const glm::vec3 &translation);
            void matrix(const glm::mat4 &matrix);
        protected:
            void decompose() const;

            glm::mat4 _matrix;
            glm::quat _orientation;
            glm::vec3 _scale;
            glm::vec3 _translation;

            bool _validMatrix;
            bool _decomposed;
    };
}
