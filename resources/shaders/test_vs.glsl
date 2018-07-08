#version 450

layout(location=0) in vec3 pos;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 posi;

void main() {
   mat4 MVP = projectionMatrix * viewMatrix;
   posi = pos;
   gl_Position = MVP * vec4(pos,1.0);
}
