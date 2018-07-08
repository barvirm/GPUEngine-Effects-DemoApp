#version 450

layout(location=0) in vec3 pos;
layout(location=2) in vec2 texcoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec2 uv;

void main() {
   mat4 MVP = projectionMatrix * viewMatrix * modelMatrix ;
   uv = texcoords;
   gl_Position = MVP * vec4(pos,1.0);
}