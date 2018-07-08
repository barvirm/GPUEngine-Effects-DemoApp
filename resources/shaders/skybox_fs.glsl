#version 430
layout(location = 0) out vec4 fragColor;
layout(binding  = 0) uniform samplerCube skybox;

uniform uvec2 WindowSize;
uniform mat4 InverseProjectionViewMatrix;

void main() {
    vec2 coord = gl_FragCoord.xy / vec2(WindowSize);
    vec3 view  = normalize((InverseProjectionViewMatrix*vec4( -1 + 2*coord, -1, 1)).xyz);
    fragColor = texture(skybox, view);
    return;
}
