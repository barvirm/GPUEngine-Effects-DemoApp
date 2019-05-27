#version 430

in vec3 vNormal;
in vec3 vFracPosition;

out vec4 FragColor;

vec3 lightPos = vec3(5,5,5); 

const float ambientStrength = 0.3f;
const vec3 lightColor = vec3(1);

void main() {

    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - vFracPosition); 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 ambient = ambientStrength * lightColor;
    vec3 result = (ambient + diffuse) * vec3(1);
    FragColor = vec4(result, 1.0);

}