#version 330 core

in vec3 norm;
in vec3 fragPos;

out vec4 color;

uniform vec3 light;
uniform vec3 lightPos;

void main()
{
    vec3 ambientLight = 0.2f * light;

    vec3 lightDirection = normalize(lightPos - fragPos);
    // note: angle is actually cosined!
    float angle = max(dot(norm, lightDirection), 0.0f);
    vec3 diffuse = angle * light;

    color = vec4((ambientLight + diffuse) * vec3(1, 1, 1), 1);
}
