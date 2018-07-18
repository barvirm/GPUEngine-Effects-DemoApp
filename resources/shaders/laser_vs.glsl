#version 450

layout(location=0) in vec3 pos;

out VSout {
    int index;
} vsout;

void main() {
    vsout.index = gl_VertexID/2;
    gl_Position = vec4(pos, 1.0);
}
