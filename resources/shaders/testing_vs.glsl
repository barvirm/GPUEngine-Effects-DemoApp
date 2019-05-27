#version 430

// const float ambientStrength = 0.3f;
// const vec3 lightColor = vec3(0);

layout (location = 0) in vec3 vPosition;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;


out vec3 vNormal;
out vec3 vFracPosition;

void main() {
    vNormal = vPosition;
    vFracPosition = (modelMatrix * vec4(vPosition, 1.0)).xyz;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vPosition, 1.0);
}