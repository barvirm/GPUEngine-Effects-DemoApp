#version 450

layout(location=0)out vec4 fragColor;

uniform layout(binding = 0) sampler2D diffTex;
uniform vec4 color;

in vec2 uv;

void main()
{
    vec4 c = color;
    c.a = 1.0f;
    fragColor = texture(diffTex,uv) * c;
}