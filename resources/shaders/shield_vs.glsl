#version 450

layout(location = 0) in vec3 pos;

layout(std430, binding = 4) readonly buffer ModelMatrixies { mat4 modelMatrix[]; };

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 vPosition;
out int glInstanceID;

void main() {
    vPosition = pos;
    glInstanceID = gl_InstanceID;

    mat4 MVP =  projectionMatrix * viewMatrix * modelMatrix[gl_InstanceID] ;
    gl_Position =  MVP * vec4(pos, 1.0);
}
