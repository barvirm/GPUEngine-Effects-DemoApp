#version 450

in vec3 posi;
out vec4 fragColor;

void main()
{
   fragColor = vec4(posi, 1.0);
}
