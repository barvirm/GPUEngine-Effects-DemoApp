#include <sg/SceneGraph/Transform>


msg::Transform::Transform()
    : _orientation( glm::quat(0.0f, 0.0f, 0.0f, 1.0f) )
    , _scale( glm::vec3(1.0f, 1.0f, 1.0f) )
    , _transform( glm::vec3(0.0f, 0.0f, 0.0f) )
    , _validMatrix(false)
    , _decomposed(true) 
    {};

glm::vec3 msg::Transform::orientation() const {
    if (!_decomposed) decompose();
    return _orientation;
}

glm::vec3 msg::Transform::scale() const {
    if (!_decomposed) decompose();
    return _scale;
}

glm::vec3 msg::Transform::translation() const {
    if (!_decomposed) decompose();
    return _translation;
}

glm::mat4 msg::Transform::matrix() const {
    if (!_validMatrix) {
        glm::mat4 rotation = glm::toMat4(_orientation);
        glm::mat4 translation = glm::translate(glm::mat4(), _translation);
        glm::mat4 scale = glm::scale(glm::mat4(), _scale);
        _matrix = translation * rotation * scale;
        _validMatrix = true;
    }
    return _matrix;

}

glm::mat4 msg::Transform::inverseMatrix() const {
    if (!_decomposed) decompose();
    return glm::inverse(matrix());
}

// settors
void msg::Transform::orientation(const glm::quat &orientation) {
    if (!_decomposed) decompose();
    _validMatrix = false;
    _orientation = orientation;
}

void msg::Transform::scale(const glm::vec3 &scale) {
    if (!_decomposed) decompose();
    _validMatrix = false;
    _scale = scale;
}
void msg::Transform::translation(const glm::vec3 &translation) {
    if (!_decomposed) decompose();
    _validMatrix = false;
    _translation = translation;
}
void msg::Transform::matrix(const glm::mat4 &matrix) {
    _validMatrix = true;
    _decomposed = false;
    _matrix = matrix;
}

void msg::Transform::decompose() const {
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(_matrix, _scale, _orientation, _translation, skew, perspective);
    _decomposed = true;
}

